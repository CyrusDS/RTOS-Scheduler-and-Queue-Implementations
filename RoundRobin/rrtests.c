#include <stdlib.h>
#include "ctest.h"
#include "rr.h"

CTEST_DATA(roundrobin) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(roundrobin) {
    int execution[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 2;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

CTEST2(roundrobin, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// TODO add additional tests to help debug
// assert execcution times set
CTEST2(roundrobin, test_execution_time) {
    for (int i = 1; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i-1].execution_time);
    }
}
//check average wait time
CTEST2(roundrobin, test_average_wait_time) {
    float wait = calculate_average_wait_time(data->task,data->size);
    ASSERT_EQUAL((float)1.33f, wait); // 0 + 1 + 3 / 3
}
//check average turnaround time
CTEST2(roundrobin, test_average_turnaround_time) {
    float turnaround = calculate_average_turn_around_time(data->task,data->size);
    ASSERT_EQUAL((float)3.33f, turnaround);
}
//assert individual wait times
CTEST2(roundrobin, test_individual_wait_time) {
    ASSERT_EQUAL(0,data->task[0].waiting_time); 
    ASSERT_EQUAL(1,data->task[1].waiting_time); 
    ASSERT_EQUAL(3,data->task[2].waiting_time); 
}
//assert individual turnaround times
CTEST2(roundrobin, test_individual_turnaround_time) {
    ASSERT_EQUAL(1,data->task[0].turnaround_time); 
    ASSERT_EQUAL(3,data->task[1].turnaround_time); 
    ASSERT_EQUAL(6,data->task[2].turnaround_time); 
}

CTEST_DATA(roundrobin2) {
    struct task_t task[5];
    int size;
};

CTEST_SETUP(roundrobin2) {
    int execution[] = {1, 2, 3, 4, 5};
    data->size = sizeof(execution) / sizeof(execution[0]);
    int quantum = 1;

    init(data->task, execution, data->size);
    round_robin(data->task, quantum, data->size);
}

CTEST2(roundrobin2, test_process2) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}
//assert individual wait times
CTEST2(roundrobin2, test_individual_wait_time2) {
    ASSERT_EQUAL(0,data->task[0].waiting_time); 
    ASSERT_EQUAL(4,data->task[1].waiting_time); 
    ASSERT_EQUAL(7,data->task[2].waiting_time);
    ASSERT_EQUAL(9,data->task[3].waiting_time);
    ASSERT_EQUAL(10,data->task[4].waiting_time); 

}
//check average wait time
CTEST2(roundrobin2, test_average_wait_time2) {
    float wait = calculate_average_wait_time(data->task,data->size);
    ASSERT_EQUAL((float)6.0f, wait); 
}

