
#include <assert.h>
#define _BSD_SOURCE 1
#include <stdlib.h>
#include <strings.h>
#include "form.h"
#include "skiplist.h"

long int random(void);

s_skiplist_node * new_skiplist_node (void *value, unsigned long height)
{
        s_skiplist_node *n = malloc(sizeof(s_skiplist_node) +
                                    height * sizeof(void*));
        if (n) {
                n->type = FORM_SKIPLIST_NODE;
                n->value = value;
                n->height = height;
                bzero(skiplist_node_links(n), height * sizeof(void*));
        }
        return n;
}

s_skiplist_node * skiplist_node_next_ (s_skiplist_node *n,
                                       unsigned long height)
{
        return skiplist_node_links(n)[height];
}

/*
  Random height
  -------------

  ∀ U ∈ ℕ : 1 < U
  ∀ n ∈ ℕ*
  ∀ r ∈ ℕ : r ≤ n
  ∀ random : ℕ* ⟶ ℕ
             random(n) ∈ [0..n-1]
             ∀ i ∈ [0..n-1], P(random(n) = i) = n⁻¹               (i)
  Qᵣ := random(Uⁿ) < Uⁿ⁻ʳ

  (i) ⇒        P(random(n) < v) = ∑ᵢ₌₀ᵛ⁻¹ P(random(n) = i)
      ⇒        P(random(n) < v) = v . n⁻¹                        (ii)

      ⇒    P(random(Uⁿ) < Uⁿ⁻ʳ) = Uⁿ⁻ʳ . (Uⁿ)⁻¹
      ⇔                   P(Qᵣ) = U⁻ʳ                           (iii)

  P(Qₙ) = P(random(Uⁿ) < U⁰)
        = P(random(Uⁿ) < 1)
        = P(random(Uⁿ) = 0)
        = U⁻ⁿ

  R := maxᵣ(Qᵣ)
     = maxᵣ(random(Uⁿ) < Uⁿ⁻ʳ)
     = maxᵣ(random(Uⁿ) + 1 ≤ Uⁿ⁻ʳ)
     = maxᵣ(logᵤ(random(Uⁿ) + 1) ≤ n - r)
     = maxᵣ(⌈logᵤ(random(Uⁿ) + 1)⌉ ≤ n - r)
     = maxᵣ(r ≤ n - ⌈logᵤ(random(Uⁿ) + 1)⌉)
     = n - ⌈logᵤ(random(Uⁿ) + 1)⌉                                (iv)

                       0 ≤ random(Uⁿ) < Uⁿ
   ⇔                   1 ≤ random(Uⁿ)+1 ≤ Uⁿ
   ⇔        logᵤ(1) ≤ logᵤ(random(Uⁿ)+1) ≤ logᵤ(Uⁿ)
   ⇔             0 ≤ ⌈logᵤ(random(Uⁿ)+1)⌉ ≤ n
   ⇔           -n ≤ -⌈logᵤ(random(Uⁿ)+1)⌉ ≤ 0
   ⇔         0 ≤ n - ⌈logᵤ(random(Uⁿ)+1)⌉ ≤ n
   ⇔                      0 ≤ R ≤ n                               (v)
*/

static void skiplist_height_table_ (s_skiplist *sl, double spacing)
{
        unsigned h;
        double w = spacing;
        double end = w;
        for (h = 0; h < sl->max_height; h++) {
                w *= spacing;
                end += w;
                skiplist_height_table(sl)[h] = end;
        }
}

s_skiplist * new_skiplist (int max_height, double spacing)
{
        s_skiplist *sl = malloc(sizeof(s_skiplist) +
                                max_height * sizeof(long));
        if (sl) {
                sl->type = FORM_SKIPLIST;
                sl->head = new_skiplist_node(NULL, max_height);
                sl->compare = skiplist_compare_ptr;
                sl->length = 0;
                sl->max_height = max_height;
                skiplist_height_table_(sl, spacing);
        }
        return sl;
}

int skiplist_compare_ptr (void *a, void *b)
{
        if (a < b)
                return -1;
        if (a > b)
                return 1;
        return 0;
}

s_skiplist_node * skiplist_pred (s_skiplist *sl, void *value)
{
        int level = sl->max_height;
        s_skiplist_node *pred = new_skiplist_node(NULL,
                                                  sl->max_height);
        s_skiplist_node *node = sl->head;
        assert(pred);
        while (level--) {
                s_skiplist_node *n = skiplist_node_next(node, level);
                while (n && sl->compare(n->value, value) < 0) {
                        node = n;
                        n = skiplist_node_next(node, level);
                }
                skiplist_node_next(pred, level) = node;
        }
        return pred;
}

void skiplist_node_insert (s_skiplist_node *n, s_skiplist_node *pred)
{
        unsigned level;
        for (level = 0; level < n->height; level++) {
                s_skiplist_node *p = skiplist_node_next(pred, level);
                skiplist_node_next(n, level) =
                        skiplist_node_next(p, level);
                skiplist_node_next(p, level) = n;
        }
}

unsigned skiplist_random_height (s_skiplist *sl)
{
        long max = skiplist_height_table(sl)[sl->max_height - 1];
        long k = random() % max;
        int i;
        for (i = 0; k > skiplist_height_table(sl)[i]; i++)
                ;
        return sl->max_height - i;
}

s_skiplist_node * skiplist_insert (s_skiplist *sl, void *value)
{
        s_skiplist_node *pred = skiplist_pred(sl, value);
        s_skiplist_node *next = skiplist_node_next(pred, 0);
        unsigned height;
        s_skiplist_node *n;
        next = skiplist_node_next(next, 0);
        if (next && sl->compare(value, next->value) == 0)
                return next;
        height = skiplist_random_height(sl);
        n = new_skiplist_node(value, height);
        skiplist_node_insert(n, pred);
        sl->length++;
        return n;
}

void * skiplist_delete (s_skiplist *sl, void *x)
{
        unsigned long level;
        s_skiplist_node *pred;
        s_skiplist_node *next;
        void *value;
        pred = skiplist_pred(sl, x);
        assert(pred);
        next = skiplist_node_next(pred, 0);
        assert(next);
        next = skiplist_node_next(next, 0);
        if (!next || sl->compare(x, next->value) != 0)
                return NULL;
        for (level = 0; level < next->height; level++) {
                s_skiplist_node *p = skiplist_node_next(pred, level);
                skiplist_node_next(p, level) =
                        skiplist_node_next(next, level);
        }
        value = next->value;
        sl->length--;
        return value;
}

s_skiplist_node * skiplist_find (s_skiplist *sl, void *value)
{
        s_skiplist_node *node = sl->head;
        int level = node->height;
        while (level--) {
                s_skiplist_node *n = node;
                int c;
                while (n && (c = sl->compare(n->value, value)) < 0)
                        n = skiplist_node_next(n, level);
                if (c == 0)
                        return n;
        }
        return NULL;
}
