
#include <assert.h>
#include <check.h>
#include "compare.h"
#include "skiplist.h"

s_skiplist *g_sl = NULL;

void setup_inserts ()
{
        g_sl = new_skiplist(5, 4);
}

void teardown_inserts ()
{
        g_sl = NULL;
}

void setup_pred ()
{
        g_sl = new_skiplist(5, 4);
        skiplist_insert(g_sl, (void*) 2);
        skiplist_insert(g_sl, (void*) 3);
}

void teardown_pred ()
{
        g_sl = NULL;
}

void setup_deletes ()
{
        g_sl = new_skiplist(5, 4);
        skiplist_insert(g_sl, (void*) 1);
        skiplist_insert(g_sl, (void*) 2);
        skiplist_insert(g_sl, (void*) 3);
        skiplist_insert(g_sl, (void*) 4);
        skiplist_insert(g_sl, (void*) 5);
        skiplist_insert(g_sl, (void*) 6);
        skiplist_insert(g_sl, (void*) 7);
        skiplist_insert(g_sl, (void*) 8);
        skiplist_insert(g_sl, (void*) 9);
        skiplist_insert(g_sl, (void*) 10);
}

void teardown_deletes ()
{
        g_sl = NULL;
}

START_TEST (test_skiplist_create)
{
        s_skiplist *sl = new_skiplist(5, 4);
        assert(sl && sl->length == 0);
}
END_TEST

START_TEST (test_skiplist_insert_one)
{
        skiplist_insert(g_sl, (void*) 1);
        skiplist_insert(g_sl, (void*) 1);
        assert(g_sl->length == 1);
        assert(skiplist_find(g_sl, (void*) 1));
        assert(skiplist_find(g_sl, (void*) 2) == 0);
}
END_TEST

START_TEST (test_skiplist_insert_two)
{
        skiplist_insert(g_sl, (void*) 1);
        skiplist_insert(g_sl, (void*) 2);
        skiplist_insert(g_sl, (void*) 1);
        skiplist_insert(g_sl, (void*) 2);
        assert(g_sl->length == 2);
        assert(skiplist_find(g_sl, (void*) 1));
        assert(skiplist_find(g_sl, (void*) 2));
        assert(skiplist_find(g_sl, (void*) 3) == 0);
}
END_TEST

START_TEST (test_skiplist_insert_ten)
{
        skiplist_insert(g_sl, (void*) 1);
        skiplist_insert(g_sl, (void*) 2);
        skiplist_insert(g_sl, (void*) 3);
        skiplist_insert(g_sl, (void*) 4);
        skiplist_insert(g_sl, (void*) 5);
        skiplist_insert(g_sl, (void*) 6);
        skiplist_insert(g_sl, (void*) 7);
        skiplist_insert(g_sl, (void*) 8);
        skiplist_insert(g_sl, (void*) 9);
        skiplist_insert(g_sl, (void*) 10);
        assert(g_sl->length == 10);
        assert(skiplist_find(g_sl, (void*) 1));
        assert(skiplist_find(g_sl, (void*) 2));
        assert(skiplist_find(g_sl, (void*) 3));
        assert(skiplist_find(g_sl, (void*) 4));
        assert(skiplist_find(g_sl, (void*) 5));
        assert(skiplist_find(g_sl, (void*) 6));
        assert(skiplist_find(g_sl, (void*) 7));
        assert(skiplist_find(g_sl, (void*) 8));
        assert(skiplist_find(g_sl, (void*) 9));
        assert(skiplist_find(g_sl, (void*) 10));
        assert(skiplist_find(g_sl, (void*) 11) == 0);
}
END_TEST

START_TEST (test_skiplist_pred_before_first)
{
        s_skiplist_node *pred = skiplist_pred(g_sl, (void*) 1);
        unsigned long level;
        for (level = 0; level < pred->height; level++)
                assert(skiplist_node_next(pred, level) == g_sl->head);
}
END_TEST

START_TEST (test_skiplist_pred_first)
{
        s_skiplist_node *pred;
        s_skiplist_node *p;
        unsigned long level;
        unsigned long height;
        pred = skiplist_pred(g_sl, (void*) 2);
        assert(pred);
        p = skiplist_node_next(g_sl->head, 0);
        assert(p);
        assert(p->value == (void*) 2);
        height = p->height;
        for (level = 0; level < height; level++) {
                p = skiplist_node_next(pred, level);
                assert(p == g_sl->head);
                assert(skiplist_node_next(p, level));
                assert(skiplist_node_next(p, level)->value == (void*) 2);
        }
}
END_TEST

START_TEST (test_skiplist_pred_last)
{
        s_skiplist_node *pred;
        s_skiplist_node *p;
        unsigned long level;
        unsigned long height;
        pred = skiplist_pred(g_sl, (void*) 3);
        assert(pred);
        p = skiplist_node_next(pred, 0);
        assert(p);
        p = skiplist_node_next(p, 0);
        assert(p);
        assert(p->value == (void*) 3);
        height = p->height;
        for (level = 0; level < height; level++) {
                p = skiplist_node_next(pred, level);
                assert(p);
                assert(skiplist_node_next(p, level));
                assert(skiplist_node_next(p, level)->value == (void*) 3);
        }
}
END_TEST

START_TEST (test_skiplist_pred_after_last)
{
        s_skiplist_node *pred;
        s_skiplist_node *p;
        unsigned long level;
        unsigned long height;
        pred = skiplist_pred(g_sl, (void*) 4);
        assert(pred);
        p = skiplist_node_next(pred, 0);
        assert(p);
        assert(p->value == (void*) 3);
        height = p->height;
        for (level = 0; level < height; level++) {
                p = skiplist_node_next(pred, level);
                assert(p);
                assert(p->value == (void*) 3);
                assert(skiplist_node_next(p, level) == 0);
        }
}
END_TEST

START_TEST (test_skiplist_delete_first)
{
        assert(g_sl->length == 10);
        assert((void*) 1 == skiplist_delete(g_sl, (void*) 1));
        assert(g_sl->length == 9);
}
END_TEST

START_TEST (test_skiplist_delete_last)
{
        assert((void*) 10 == skiplist_delete(g_sl, (void*) 10));
        assert(g_sl->length == 9);
}
END_TEST

START_TEST (test_skiplist_delete_middle)
{
        assert((void*) 5 == skiplist_delete(g_sl, (void*) 5));
        assert(g_sl->length == 9);
}
END_TEST

START_TEST (test_skiplist_delete_all)
{
        assert((void*) 1 == skiplist_delete(g_sl, (void*) 1));
        assert((void*) 2 == skiplist_delete(g_sl, (void*) 2));
        assert((void*) 3 == skiplist_delete(g_sl, (void*) 3));
        assert((void*) 4 == skiplist_delete(g_sl, (void*) 4));
        assert((void*) 5 == skiplist_delete(g_sl, (void*) 5));
        assert((void*) 6 == skiplist_delete(g_sl, (void*) 6));
        assert((void*) 7 == skiplist_delete(g_sl, (void*) 7));
        assert((void*) 8 == skiplist_delete(g_sl, (void*) 8));
        assert((void*) 9 == skiplist_delete(g_sl, (void*) 9));
        assert((void*) 10 == skiplist_delete(g_sl, (void*) 10));
        assert(g_sl->length == 0);
}
END_TEST

Suite * skiplist_suite(void)
{
    Suite *s;
    TCase *tc_core;
    TCase *tc_inserts;
    TCase *tc_pred;
    TCase *tc_deletes;
    s = suite_create("Skiplist");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_skiplist_create);
    suite_add_tcase(s, tc_core);
    tc_inserts = tcase_create("Inserts");
    tcase_add_checked_fixture(tc_inserts, setup_inserts, teardown_inserts);
    tcase_add_test(tc_inserts, test_skiplist_insert_one);
    tcase_add_test(tc_inserts, test_skiplist_insert_two);
    tcase_add_test(tc_inserts, test_skiplist_insert_ten);
    suite_add_tcase(s, tc_inserts);
    tc_pred = tcase_create("Pred");
    tcase_add_checked_fixture(tc_pred, setup_pred, teardown_pred);
    tcase_add_test(tc_pred, test_skiplist_pred_before_first);
    tcase_add_test(tc_pred, test_skiplist_pred_first);
    tcase_add_test(tc_pred, test_skiplist_pred_last);
    tcase_add_test(tc_pred, test_skiplist_pred_after_last);
    suite_add_tcase(s, tc_pred);
    tc_deletes = tcase_create("Deletes");
    tcase_add_checked_fixture(tc_deletes, setup_deletes, teardown_deletes);
    tcase_add_test(tc_deletes, test_skiplist_delete_first);
    tcase_add_test(tc_deletes, test_skiplist_delete_last);
    tcase_add_test(tc_deletes, test_skiplist_delete_middle);
    tcase_add_test(tc_deletes, test_skiplist_delete_all);
    suite_add_tcase(s, tc_deletes);
    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = skiplist_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}
