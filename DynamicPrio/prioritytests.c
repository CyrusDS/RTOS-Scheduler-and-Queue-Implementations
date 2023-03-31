#include <stdlib.h>
#include "ctest.h"
#include "priority.h"
#include <stdio.h>

CTEST_DATA(priority) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(priority) {
    int execution[] = {1, 2, 3};
    int priority[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, priority, data->size);
    priority_schedule(data->task, data->size);
}
//key: 1(2) :  1 = exec time, 2 = priority in queue (should always be increasing)
// 1(1) , 2(2) , 3(3) initially t = 0, w1 = 0, w2 = 0, w3 = 0
// pop element since equal to quantum
// 2(2), 3(3), w1 = 0, w2 = 1, w3 = 1
//use one time quantum and reorder processes 
// 1(2), 3(3), t = 1 .... 
// 1(2) == 1 --> 1(4) (left_time == time),  3(3) != 1
// after reorder
// 3(3), 1(4), w1 = 0, w2 = 1, w3 = 2
// use one time quantum and reorder processes
// 2(3), 1(4), t = 2 ....
// 2(3) == 2 --> 2(6) (left_time == time), 1(4) == 2 --> 1(16) (exec_time == time)
//after reorder
// 2(6), 1(16), w1 = 0, w2 = 2, w3 = 2
//use one time quantumn and reorder processes
// 1(6), 1(16), t = 3 ....
// 1(6) == 3 --> 1(24) (exec_time == time)
//after reorder
//1(16), 1(24), w1 = 0, w2 = 3, w3 = 2
// pop element since equal to quantum
// 1(24), t = 4, w1 = 0, w2 = 3, w3 = 3
// turn1 = 0+1, turn2 = 2 + 3, turn3 = 3 + 3

CTEST2(priority, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}
// TODO add additional tests to help debug
CTEST2(priority, test_execution_time) {
    for (int i = 1; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i-1].execution_time);
    }
}
//assert individual wait times
CTEST2(priority, test_individual_wait_time) {
    ASSERT_EQUAL(0,data->task[0].waiting_time); 
    ASSERT_EQUAL(2,data->task[1].waiting_time); 
    ASSERT_EQUAL(3,data->task[2].waiting_time); 
}
//assert individual turnaround times
CTEST2(priority, test_individual_turnaround_time) {
    ASSERT_EQUAL(1,data->task[0].turnaround_time); 
    ASSERT_EQUAL(4,data->task[1].turnaround_time); 
    ASSERT_EQUAL(6,data->task[2].turnaround_time); 
}

//check average wait time
CTEST2(priority, test_average_wait_time) {
    float wait = calculate_average_wait_time(data->task,data->size);
    ASSERT_EQUAL((float)1.66F, wait); // 0 + 1 + 3 / 3
}
//check average turnaround time
CTEST2(priority, test_average_turnaround_time) {
    float turnaround = calculate_average_turn_around_time(data->task,data->size);
    ASSERT_EQUAL((float)3.66f, turnaround);
}

CTEST2(priority, increasing_order) { //check to ensure sorted by priority always
    int pass = 0;
    for (int i = 0; i < data->size; i++) {
        for (int j = 0; j < i; j++) {
            pass = ((int)data->task[j].priority <= (int)data->task[i].priority);
            // printf("\n %d vs %d",(int)data->task[j].priority,(int)data->task[i].priority); 
            ASSERT_EQUAL(1, pass);
            }
    }
}
CTEST2(priority, left_to_execute) { //ensure all tasks finished
    for (int i = 1; i < data->size; i++) {
        ASSERT_EQUAL(0, data->task[i-1].left_to_execute);
    }
}
// // TODO add additional tests to help debug
// CTEST_DATA(priority2) {
//     struct task_t task[5];
//     int size;
// };
// CTEST_SETUP(priority2) {
//     int execution[] = {1, 2, 3, 5, 4};
//     int priority[] = {6, 2, 3, 2, 1};
//     data->size = sizeof(execution) / sizeof(execution[0]);
//     init(data->task, execution, priority, data->size);
//     priority_schedule(data->task, data->size);
// }
// CTEST2(priority2, test_process) { //INITIAL task organization to assign each id based on initial prio
//     ASSERT_EQUAL(4, (int)data->task[0].process_id);
//     ASSERT_EQUAL(1, (int)data->task[1].process_id);
//     ASSERT_EQUAL(3, (int)data->task[2].process_id);
//     ASSERT_EQUAL(2, (int)data->task[3].process_id);
//     ASSERT_EQUAL(0, (int)data->task[4].process_id);
// }
//key: 1(2){3} :  1 = exec time, 2 = priority in queue (should always be increasing), 3 = ID (decided upon first process organization)
// 4(1){0}, 2(2){1}, 5(2){2}, 3(3){3}, 1(6){4} , initially t = 0
//use one time quantum and reorder processes 
// 3(1){0}, 2(2){1}, 5(2){2}, 3(3){3}, 1(6){4}, t = 1 ....
// 1(6){4} == 1 --> 1(24){4} (exec_time == time) , (left_time == time)
//after reorder
// 3(1){0}, 2(2){1}, 5(2){2}, 3(3){3}, 1(24){4} , w0=0,w1=1,w2=1,w3=1,w4=1
//use one time quantum and reorder processes 
// 2(1){0}, 2(2){1}, 5(2){2}, 3(3){3}, 1(24){4}, t = 2 ....
// 2(1){0} == 2 --> 2(4){0}  (left_time == time) , 2(2){1} == 2 --> 2(8){1} (exec_time == time) 
//after reorder
// 5(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=0, w1=2,w2=2,w3=2,w4=2
//use one time quantum and reorder processes 
// 4(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 3 ....
// 3(3){3} == 2 --> 3(12){3} (exec_time == time)
//after reorder
// 4(2){2},  2(4){0}, 2(8){1}, 3(12){3}, 1(24){4}, w0=1,w1=3,w2=2,w3=3,w4=3
// use one time quantum and reorder processes
// 3(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 4 ....
// no reordering?
// 3(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=2,w1=4,w2=2,w3=4,w4=4
//use one time quantum and reorder processes
// 2(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 5 ....
// no reordering? 
// 2(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=3,w1=5,w2=2,w3=5,w4=5
//use one time quantum and reorder processes
// 1(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 6 ....
// no reordering? 
// 1(2){2}, 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=4,w1=6,w2=2,w3=6,w4=6
//pop element off
// 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 7 ....
// 3(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=5,w1=7,w2=2,w3=7,w4=7
//use one time quantum and reorder processes
// 2(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 8 ....
// no reordering? 
// 2(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=6,w1=8,w2=2,w3=7,w4=8
//use one time quantum and reorder processes
// 1(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, t = 9 ....
// no reordering? 
// 1(3){3}, 2(4){0}, 2(8){1}, 1(24){4}, w0=7,w1=9,w2=2,w3=7,w4=9
// pop element off
// 2(4){0}, 2(8){1}, 1(24){4}, t = 10 ....
// 2(4){0}, 2(8){1}, 1(24){4}, w0=8,w1=10,w2=2,w3=7,w4=10
//use one time quantum and reorder processes
// 1(4){0}, 2(8){1}, 1(24){4}, t = 11 ....
// no reordering? 
// 1(4){0}, 2(8){1}, 1(24){4},  w0=8,w1=11,w2=2,w3=7,w4=10
//pop element off
// 2(8){1}, 1(24){4}, t = 12 ....
// 2(8){1}, 1(24){4}, w0=8,w1=12,w2=2,w3=7,w4=11
//use one time quantum and reorder processes
// 1(8){1}, 1(24){4}, t = 13 ....
// 1(8){1}, 1(24){4}, w0=8,w1=12,w2=2,w3=7,w4=12
// pop element off
//  1(24){4}, t = 14 ....
//  1(24){4}, w0=8,w1=12,w2=2,w3=7,w4=13
// pop element off
// t0 = 8 + 4, t1 = 12 + 2, t2 = 2 + 5, t3 = 7 + 3, t4 = 13 + 1


// CTEST2(priority2, test_individual_wait_time) {
//     ASSERT_EQUAL(8,data->task[0].waiting_time); 
//     ASSERT_EQUAL(12,data->task[1].waiting_time); 
//     ASSERT_EQUAL(2,data->task[2].waiting_time); 
//     ASSERT_EQUAL(7,data->task[3].waiting_time); 
//     ASSERT_EQUAL(13,data->task[4].waiting_time); 
// }
//assert individual turnaround times
// CTEST2(priority2, test_individual_turnaround_time) {
//     ASSERT_EQUAL(12,data->task[0].turnaround_time); 
//     ASSERT_EQUAL(14,data->task[1].turnaround_time); 
//     ASSERT_EQUAL(7,data->task[2].turnaround_time);
//     ASSERT_EQUAL(10,data->task[3].turnaround_time);
//     ASSERT_EQUAL(14,data->task[4].turnaround_time);
// }

// CTEST2(priority2, increasing_order2) { //check to ensure sorted by prio
//     int pass = 0;
//     for (int i = 0; i < data->size; i++) {
//         for (int j = 0; j < i; j++) {
//             pass = ((int)data->task[j].priority <= (int)data->task[i].priority);
//             // printf("\n %d vs %d",(int)data->task[j].execution_time,(int)data->task[i].execution_time); 
//             ASSERT_EQUAL(1, pass);
//             }
//     }
// }