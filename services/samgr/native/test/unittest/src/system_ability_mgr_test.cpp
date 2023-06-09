/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "if_system_ability_manager.h"
#include "iservice_registry.h"
#include "itest_transaction_service.h"
#include "sa_status_change_mock.h"
#include "string_ex.h"
#include "system_ability_definition.h"
#include "system_ability_mgr_test.h"
#include "test_log.h"

#define private public
#include "system_ability_manager.h"

using namespace std;
using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace OHOS {
namespace {
constexpr int32_t TEST_VALUE = 2021;
constexpr int32_t TEST_REVERSE_VALUE = 1202;
constexpr int32_t REPEAT = 10;
constexpr int32_t TEST_EXCEPTION_HIGH_SA_ID = LAST_SYS_ABILITY_ID + 1;
constexpr int32_t TEST_EXCEPTION_LOW_SA_ID = FIRST_SYS_ABILITY_ID - 1;
const std::u16string SAMANAGER_INTERFACE_TOKEN = u"ohos.samgr.accessToken";
}
void SystemAbilityMgrTest::SetUpTestCase()
{
    DTEST_LOG << "SetUpTestCase" << std::endl;
}

void SystemAbilityMgrTest::TearDownTestCase()
{
    DTEST_LOG << "TearDownTestCase" << std::endl;
}

void SystemAbilityMgrTest::SetUp()
{
    DTEST_LOG << "SetUp" << std::endl;
}

void SystemAbilityMgrTest::TearDown()
{
    DTEST_LOG << "TearDown" << std::endl;
}

/**
 * @tc.name: AddSystemAbility001
 * @tc.desc: add system ability, input invalid parameter
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, AddSystemAbility001, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    int32_t result = sm->AddSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, nullptr);
    DTEST_LOG << "add TestTransactionService result = " << result << std::endl;
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: AddSystemAbility002
 * @tc.desc: add system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, AddSystemAbility002, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    int32_t result = sm->AddSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, testAbility);
    DTEST_LOG << "add TestTransactionService result = " << result << std::endl;
    EXPECT_EQ(result, ERR_OK);
    sm->RemoveSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
}

/**
 * @tc.name: AddSystemAbility003
 * @tc.desc: add system ability saId exception.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, AddSystemAbility003, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    int32_t result = sm->AddSystemAbility(TEST_EXCEPTION_HIGH_SA_ID, testAbility);
    EXPECT_TRUE(result != ERR_OK);
    result = sm->AddSystemAbility(TEST_EXCEPTION_LOW_SA_ID, testAbility);
    EXPECT_TRUE(result != ERR_OK);
    sm->RemoveSystemAbility(TEST_EXCEPTION_HIGH_SA_ID);
    sm->RemoveSystemAbility(TEST_EXCEPTION_LOW_SA_ID);
}

/**
 * @tc.name: AddSystemAbility004
 * @tc.desc: add system ability with empty capability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, AddSystemAbility004, TestSize.Level1)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_TT_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    ISystemAbilityManager::SAExtraProp saExtraProp(false, ISystemAbilityManager::DUMP_FLAG_PRIORITY_DEFAULT,
        u"", u"");
    int32_t ret = sm->AddSystemAbility(systemAbilityId, new TestTransactionService(), saExtraProp);
    EXPECT_EQ(ret, ERR_OK);
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: AddSystemAbility005
 * @tc.desc: add system ability with validated capability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, AddSystemAbility005, TestSize.Level1)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_SO_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    std::u16string capability = u"{\"Capabilities\":{\"aaa\":\"[10.4, 20.5]\",\"bbb\":\"[11, 55]\",\
        \"ccc\":\"this is string\", \"ddd\":\"[aa, bb, cc, dd]\", \"eee\":5.60, \"fff\":4545, \"ggg\":true}}";
    ISystemAbilityManager::SAExtraProp saExtraProp(true, ISystemAbilityManager::DUMP_FLAG_PRIORITY_DEFAULT,
        capability, u"");
    int32_t ret = sm->AddSystemAbility(systemAbilityId, new TestTransactionService(), saExtraProp);
    EXPECT_EQ(ret, ERR_OK);
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: RemoveSystemAbility001
 * @tc.desc: remove not exist system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, RemoveSystemAbility001, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    int32_t result = sm->RemoveSystemAbility(0);
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: RemoveSystemAbility002
 * @tc.desc: remove system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, RemoveSystemAbility002, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    sm->AddSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, testAbility);
    int32_t result = sm->RemoveSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
    EXPECT_EQ(result, ERR_OK);
}

/**
 * @tc.name: GetSystemAbility001
 * @tc.desc: get not exist system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, GetSystemAbility001, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    auto ability = sm->GetSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
    EXPECT_EQ(ability, nullptr);
}

/**
 * @tc.name: GetSystemAbility002
 * @tc.desc: get system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, GetSystemAbility002, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    sm->AddSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, testAbility);
    auto ability = sm->GetSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
    EXPECT_TRUE(ability != nullptr);
    sm->RemoveSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
}

/**
 * @tc.name: GetSystemAbility003
 * @tc.desc: get system ability and then transaction.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, GetSystemAbility003, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    sm->AddSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, testAbility);
    auto ability = sm->GetSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
    EXPECT_TRUE(ability != nullptr);
    sptr<ITestTransactionService> targetAblility = iface_cast<ITestTransactionService>(ability);
    EXPECT_TRUE(targetAblility != nullptr);
    int32_t rep = 0;
    int32_t result = targetAblility->ReverseInt(TEST_VALUE, rep);
    DTEST_LOG << "testAbility ReverseInt result = " << result << ", get reply = " << rep << std::endl;
    EXPECT_EQ(rep, TEST_REVERSE_VALUE);
    sm->RemoveSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID);
}

/**
 * @tc.name: GetSystemAbility004
 * @tc.desc: get system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, GetSystemAbility004, TestSize.Level2)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    for (int32_t i = 0; i < REPEAT; ++i) {
        auto result = sm->AddSystemAbility((DISTRIBUTED_SCHED_TEST_SO_ID + i), new TestTransactionService());
        EXPECT_EQ(result, ERR_OK);
    }
    for (int32_t i = 0; i < REPEAT; ++i) {
        int32_t saId = DISTRIBUTED_SCHED_TEST_SO_ID + i;
        auto saObject = sm->GetSystemAbility(saId);
        EXPECT_TRUE(saObject != nullptr);
        sm->RemoveSystemAbility(saId);
    }
}

/**
 * @tc.name: GetSystemAbility005
 * @tc.desc: get remote device system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, GetSystemAbility005, TestSize.Level2)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    string fakeDeviceId = "fake_dev";
    auto abilityObj = sm->GetSystemAbility(DISTRIBUTED_SCHED_TEST_TT_ID, fakeDeviceId);
    EXPECT_EQ(abilityObj, nullptr);
}

/**
 * @tc.name: CheckSystemAbility001
 * @tc.desc: check system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, CheckSystemAbility001, TestSize.Level1)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_TT_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<IRemoteObject> testAbility = new TestTransactionService();
    sm->AddSystemAbility(systemAbilityId, testAbility);
    sptr<IRemoteObject> abilityObj = sm->CheckSystemAbility(systemAbilityId);
    EXPECT_TRUE(abilityObj != nullptr);
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: CheckOnDemandSystemAbility001
 * @tc.desc: check on demand system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, CheckOnDemandSystemAbility001, TestSize.Level1)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_TT_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sm->AddSystemAbility(systemAbilityId, new TestTransactionService());
    int32_t ret = sm->AddOnDemandSystemAbilityInfo(systemAbilityId, u"test_localmanagername");
    EXPECT_TRUE(ret != ERR_OK);
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: CheckOnDemandSystemAbility002
 * @tc.desc: check on demand system ability.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, CheckOnDemandSystemAbility002, TestSize.Level1)
{
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    ISystemAbilityManager::SAExtraProp saExtraProp(false, 0, u"", u"");
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_TT_ID;
    int32_t result = sm->AddSystemAbility(systemAbilityId, new TestTransactionService(), saExtraProp);
    EXPECT_EQ(result, ERR_OK);
    sptr<IRemoteObject> saObject = sm->CheckSystemAbility(systemAbilityId);
    result = sm->AddOnDemandSystemAbilityInfo(systemAbilityId, u"just_test");
    EXPECT_TRUE(result != ERR_OK);
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: ListSystemAbility001
 * @tc.desc: list all system abilities.
 * @tc.type: FUNC
 * @tc.require: AR000GGVR6
 */
HWTEST_F(SystemAbilityMgrTest, ListSystemAbility001, TestSize.Level1)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_TT_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sm->AddSystemAbility(systemAbilityId, new TestTransactionService());
    auto saList = sm->ListSystemAbilities();
    EXPECT_TRUE(!saList.empty());
    auto iter = std::find(saList.begin(), saList.end(), to_utf16(std::to_string(systemAbilityId)));
    EXPECT_TRUE(iter != saList.end());
    sm->RemoveSystemAbility(systemAbilityId);
}

/**
 * @tc.name: LoadSystemAbility001
 * @tc.desc: load system ability with invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: SR000GH59P
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility001, TestSize.Level0)
{
    int32_t systemAbilityId = TEST_EXCEPTION_LOW_SA_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    int32_t result = sm->LoadSystemAbility(systemAbilityId, nullptr);
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility002
 * @tc.desc: load system ability with invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility002, TestSize.Level0)
{
    int32_t systemAbilityId = TEST_EXCEPTION_HIGH_SA_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    int32_t result = sm->LoadSystemAbility(systemAbilityId, nullptr);
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility003
 * @tc.desc: load system ability with invalid callback.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility003, TestSize.Level0)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_SO_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    int32_t result = sm->LoadSystemAbility(systemAbilityId, nullptr);
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility004
 * @tc.desc: load system ability with not exist systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P9
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility004, TestSize.Level0)
{
    int32_t systemAbilityId = DISTRIBUTED_SCHED_TEST_SO_ID;
    sptr<ISystemAbilityManager> sm = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    EXPECT_TRUE(sm != nullptr);
    sptr<ISystemAbilityLoadCallback> callback = new SystemAbilityLoadCallbackMock();
    int32_t result = sm->LoadSystemAbility(systemAbilityId, callback);
    EXPECT_TRUE(result != ERR_OK);
}

/**
 * @tc.name: LoadSystemAbility005
 * @tc.desc: test OnRemoteRequest, invalid interface token.
 * @tc.type: FUNC
 * @tc.require: AR000GH59Q
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility005, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    int32_t result = saMgr->OnRemoteRequest(ISystemAbilityManager::LOAD_SYSTEM_ABILITY_TRANSACTION,
        data, reply, option);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: LoadSystemAbility006
 * @tc.desc: test OnRemoteRequest, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility006, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(SAMANAGER_INTERFACE_TOKEN);
    MessageParcel reply;
    MessageOption option;
    int32_t result = saMgr->OnRemoteRequest(ISystemAbilityManager::LOAD_SYSTEM_ABILITY_TRANSACTION,
        data, reply, option);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: LoadSystemAbility007
 * @tc.desc: test OnRemoteRequest, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility007, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(SAMANAGER_INTERFACE_TOKEN);
    data.WriteInt32(TEST_EXCEPTION_HIGH_SA_ID);
    MessageParcel reply;
    MessageOption option;
    int32_t result = saMgr->OnRemoteRequest(ISystemAbilityManager::LOAD_SYSTEM_ABILITY_TRANSACTION,
        data, reply, option);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: LoadSystemAbility008
 * @tc.desc: test OnRemoteRequest, null callback.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility008, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(SAMANAGER_INTERFACE_TOKEN);
    data.WriteInt32(DISTRIBUTED_SCHED_TEST_SO_ID);
    MessageParcel reply;
    MessageOption option;
    int32_t result = saMgr->OnRemoteRequest(ISystemAbilityManager::LOAD_SYSTEM_ABILITY_TRANSACTION,
        data, reply, option);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: LoadSystemAbility009
 * @tc.desc: test OnRemoteRequest, not exist systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, LoadSystemAbility009, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    MessageParcel data;
    data.WriteInterfaceToken(SAMANAGER_INTERFACE_TOKEN);
    data.WriteInt32(DISTRIBUTED_SCHED_TEST_SO_ID);
    sptr<ISystemAbilityLoadCallback> callback = new SystemAbilityLoadCallbackMock();
    data.WriteRemoteObject(callback->AsObject());
    MessageParcel reply;
    MessageOption option;
    int32_t result = saMgr->OnRemoteRequest(ISystemAbilityManager::LOAD_SYSTEM_ABILITY_TRANSACTION,
        data, reply, option);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: OnRemoteDied001
 * @tc.desc: test OnRemoteDied, remove registered callback.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, OnRemoteDied001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<ISystemAbilityLoadCallback> callback = new SystemAbilityLoadCallbackMock();
    saMgr->OnAbilityCallbackDied(callback->AsObject());
    EXPECT_TRUE(saMgr->startingAbilityMap_.empty());
}

/**
 * @tc.name: StartOnDemandAbility001
 * @tc.desc: test StartOnDemandAbility, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, StartOnDemandAbility001, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->StartOnDemandAbility(TEST_EXCEPTION_LOW_SA_ID);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: StartOnDemandAbility002
 * @tc.desc: test StartOnDemandAbility, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, StartOnDemandAbility002, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->StartOnDemandAbility(TEST_EXCEPTION_HIGH_SA_ID);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: StartOnDemandAbility003
 * @tc.desc: test StartOnDemandAbility, not exist systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5TH
 */
HWTEST_F(SystemAbilityMgrTest, StartOnDemandAbility003, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->StartOnDemandAbility(DISTRIBUTED_SCHED_TEST_SO_ID);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: StartOnDemandAbility004
 * @tc.desc: test StartOnDemandAbility, not on-demand systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, StartOnDemandAbility004, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->StartOnDemandAbility(DISTRIBUTED_SCHED_SA_ID);
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: AddOnDemandSystemAbilityInfo001
 * @tc.desc: test AddOnDemandSystemAbilityInfo, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, AddOnDemandSystemAbilityInfo001, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->AddOnDemandSystemAbilityInfo(TEST_EXCEPTION_LOW_SA_ID, u"");
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: AddOnDemandSystemAbilityInfo002
 * @tc.desc: test AddOnDemandSystemAbilityInfo, invalid systemAbilityId.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, AddOnDemandSystemAbilityInfo002, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->AddOnDemandSystemAbilityInfo(TEST_EXCEPTION_HIGH_SA_ID, u"");
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: AddOnDemandSystemAbilityInfo003
 * @tc.desc: test AddOnDemandSystemAbilityInfo, invalid procName.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, AddOnDemandSystemAbilityInfo003, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->AddOnDemandSystemAbilityInfo(DISTRIBUTED_SCHED_TEST_SO_ID, u"");
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: AddOnDemandSystemAbilityInfo004
 * @tc.desc: test AddOnDemandSystemAbilityInfo, procName not registered.
 * @tc.type: FUNC
 * @tc.require: AR000GH5P8
 */
HWTEST_F(SystemAbilityMgrTest, AddOnDemandSystemAbilityInfo004, TestSize.Level0)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    int32_t result = saMgr->AddOnDemandSystemAbilityInfo(DISTRIBUTED_SCHED_TEST_SO_ID, u"fake_process_name");
    EXPECT_TRUE(result != ERR_NONE);
}

/**
 * @tc.name: OnLoadSystemAbilitySuccess001
 * @tc.desc: test OnLoadSystemAbilitySuccess, null callback.
 * @tc.type: FUNC
 * @tc.require: AR000GH59Q
 */
HWTEST_F(SystemAbilityMgrTest, OnLoadSystemAbilitySuccess001, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->NotifySystemAbilityLoaded(DISTRIBUTED_SCHED_TEST_SO_ID, nullptr, nullptr);
    EXPECT_TRUE(callback->GetSystemAbilityId() == 0);
}

/**
 * @tc.name: OnLoadSystemAbilitySuccess002
 * @tc.desc: test OnLoadSystemAbilitySuccess, null IRemoteObject.
 * @tc.type: FUNC
 * @tc.require: AR000GH59Q
 */
HWTEST_F(SystemAbilityMgrTest, OnLoadSystemAbilitySuccess002, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    saMgr->NotifySystemAbilityLoaded(DISTRIBUTED_SCHED_TEST_SO_ID, nullptr, callback);
    EXPECT_TRUE(callback->GetSystemAbilityId() == DISTRIBUTED_SCHED_TEST_SO_ID);
    EXPECT_TRUE(callback->GetRemoteObject() == nullptr);
}

/**
 * @tc.name: OnLoadSystemAbilitySuccess003
 * @tc.desc: test OnLoadSystemAbilitySuccess.
 * @tc.type: FUNC
 * @tc.require: AR000GH59Q
 */
HWTEST_F(SystemAbilityMgrTest, OnLoadSystemAbilitySuccess003, TestSize.Level1)
{
    sptr<SystemAbilityManager> saMgr = SystemAbilityManager::GetInstance();
    EXPECT_TRUE(saMgr != nullptr);
    sptr<SystemAbilityLoadCallbackMock> callback = new SystemAbilityLoadCallbackMock();
    sptr<IRemoteObject> remoteObject = new TestTransactionService();
    saMgr->NotifySystemAbilityLoaded(DISTRIBUTED_SCHED_TEST_SO_ID, remoteObject, callback);
    EXPECT_TRUE(callback->GetSystemAbilityId() == DISTRIBUTED_SCHED_TEST_SO_ID);
    EXPECT_TRUE(callback->GetRemoteObject() == remoteObject);
}
} // namespace OHOS
