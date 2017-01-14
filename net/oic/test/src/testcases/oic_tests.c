/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include "test_oic.h"

#include <os/os.h>
#include <oic/oc_api.h>

#define OIC_TAPP_PRIO       9
#define OIC_TAPP_STACK_SIZE 1024

/*
 * How long to wait before declaring discovery process failure.
 */
#define OIC_TEST_FAIL_DLY   (OS_TICKS_PER_SEC * 2)

static struct os_task oic_tapp;
static os_stack_t oic_tapp_stack[OS_STACK_ALIGN(OIC_TAPP_STACK_SIZE)];
struct os_eventq oic_tapp_evq;
static struct os_callout oic_test_timer;

static void
oic_test_timer_cb(struct os_event *ev)
{
    TEST_ASSERT_FATAL(0);
}

void
oic_test_reset_tmo(void)
{
    os_callout_reset(&oic_test_timer, OIC_TEST_FAIL_DLY);
}

static void
oic_test_handler(void *arg)
{
    test_discovery();
    tu_restart();
}

static void
oic_test_init(void)
{
    int rc;

    os_eventq_init(&oic_tapp_evq);

    /*
     * Starts tests.
     */
    os_callout_init(&oic_test_timer, &oic_tapp_evq, oic_test_timer_cb, NULL);

    rc = os_task_init(&oic_tapp, "oic_test", oic_test_handler, NULL,
                      OIC_TAPP_PRIO, OS_WAIT_FOREVER,
                      oic_tapp_stack, OS_STACK_ALIGN(OIC_TAPP_STACK_SIZE));
    TEST_ASSERT_FATAL(rc == 0);
    oc_evq_set(&oic_tapp_evq);

    os_start();
}

TEST_CASE(oic_tests)
{
    oic_test_init();
}