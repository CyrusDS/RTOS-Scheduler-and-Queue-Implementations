#include <stdlib.h>
#include "ctest.h"
#include "sjf.h"
#include <time.h>

#include <stdio.h>

CTEST_DATA(shortestjobfirst) {
    struct task_t task[3];
    int size;
};

CTEST_SETUP(shortestjobfirst) {
    int execution[] = {1, 2, 3};
    data->size = sizeof(execution) / sizeof(execution[0]);

    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}

CTEST2(shortestjobfirst, test_process) {
    for (int i = 0; i < data->size; i++) {
        ASSERT_EQUAL(i, (int)data->task[i].process_id);
    }
}

// TODO add additional tests to help debug
CTEST2(shortestjobfirst, test_execution_time) {
    for (int i = 1; i < data->size; i++) {
        ASSERT_EQUAL(i, data->task[i-1].execution_time);
    }
}
//check average wait time
CTEST2(shortestjobfirst, test_average_wait_time) {
    float wait = calculate_average_wait_time(data->task,data->size);
    ASSERT_EQUAL((float)1.33f, wait); // 0 + 1 + 3 / 3
}
//check average turnaround time
CTEST2(shortestjobfirst, test_average_turnaround_time) {
    float turnaround = calculate_average_turn_around_time(data->task,data->size);
    ASSERT_EQUAL((float)3.33f, turnaround);
}
//assert individual wait times
CTEST2(shortestjobfirst, test_individual_wait_time) {
    ASSERT_EQUAL(0,data->task[0].waiting_time); 
    ASSERT_EQUAL(1,data->task[1].waiting_time); 
    ASSERT_EQUAL(3,data->task[2].waiting_time); 
}
//assert individual turnaround times
CTEST2(shortestjobfirst, test_individual_turnaround_time) {
    ASSERT_EQUAL(1,data->task[0].turnaround_time); 
    ASSERT_EQUAL(3,data->task[1].turnaround_time); 
    ASSERT_EQUAL(6,data->task[2].turnaround_time); 
}
CTEST2(shortestjobfirst, increasing_order) { //check to ensure sorted
    int pass = 0;
    for (int i = 0; i < data->size; i++) {
        for (int j = 0; j < i; j++) {
            pass = ((int)data->task[j].execution_time <= (int)data->task[i].execution_time);
            // printf("\n %d vs %d",(int)data->task[j].execution_time,(int)data->task[i].execution_time); 
            ASSERT_EQUAL(1, pass);
            }
    }
}



CTEST_DATA(shortestjobfirst2) {
    struct task_t task[5];
    int size;
};
CTEST_SETUP(shortestjobfirst2) {
    int execution[] = {5, 2, 7, 2, 1};
    data->size = sizeof(execution) / sizeof(execution[0]);
    init(data->task, execution, data->size);
    shortest_job_first(data->task, data->size);
}
// CTEST2(shortestjobfirst2, display_sorted) {
//     for (int i = 0; i < data->size; i++) {
//         printf("\n %d",(int)data->task[i].execution_time);
//     }
// }

CTEST2(shortestjobfirst2, test_process) {
    ASSERT_EQUAL(4, (int)data->task[0].process_id);
    ASSERT_EQUAL(1, (int)data->task[1].process_id); //if two are equal, first process is first
    ASSERT_EQUAL(3, (int)data->task[2].process_id);
    ASSERT_EQUAL(0, (int)data->task[3].process_id);
    ASSERT_EQUAL(2, (int)data->task[4].process_id);
}
CTEST2(shortestjobfirst2, increasing_order2) { //check to ensure sorted
    int pass = 0;
    for (int i = 0; i < data->size; i++) {
        for (int j = 0; j < i; j++) {
            pass = ((int)data->task[j].execution_time <= (int)data->task[i].execution_time);
            // printf("\n %d vs %d",(int)data->task[j].execution_time,(int)data->task[i].execution_time); 
            ASSERT_EQUAL(1, pass);
            }
    }
}

//assert individual wait times
CTEST2(shortestjobfirst2, test_individual_wait_time2) {
    ASSERT_EQUAL(0,data->task[0].waiting_time); 
    ASSERT_EQUAL(1,data->task[1].waiting_time); 
    ASSERT_EQUAL(3,data->task[2].waiting_time);
    ASSERT_EQUAL(5,data->task[3].waiting_time);
    ASSERT_EQUAL(10,data->task[4].waiting_time); 

}
//assert individual turnaround times
CTEST2(shortestjobfirst2, test_individual_turnaround_time2) {
    ASSERT_EQUAL(1,data->task[0].turnaround_time); 
    ASSERT_EQUAL(3,data->task[1].turnaround_time); 
    ASSERT_EQUAL(5,data->task[2].turnaround_time); 
    ASSERT_EQUAL(10,data->task[3].turnaround_time);
    ASSERT_EQUAL(17,data->task[4].turnaround_time); 
}

//check average turnaround time
CTEST2(shortestjobfirst2, test_average_turnaround_time2) {
    float turnaround = calculate_average_turn_around_time(data->task,data->size);
    ASSERT_EQUAL((float)7.2f, turnaround);
}

//check average wait time
CTEST2(shortestjobfirst2, test_average_wait_time) {
    float wait = calculate_average_wait_time(data->task,data->size);
    ASSERT_EQUAL((float)3.6f, wait); // 0 + 1 + 3 / 3
}