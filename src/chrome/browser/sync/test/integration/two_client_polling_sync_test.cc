// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/macros.h"
#include "base/run_loop.h"
#include "build/build_config.h"
#include "chrome/browser/sync/test/integration/profile_sync_service_harness.h"
#include "chrome/browser/sync/test/integration/session_hierarchy_match_checker.h"
#include "chrome/browser/sync/test/integration/sessions_helper.h"
#include "chrome/browser/sync/test/integration/sync_test.h"
#include "chrome/common/webui_url_constants.h"
#include "components/browser_sync/profile_sync_service.h"
#include "components/sync/base/sync_prefs.h"
#include "components/sync/engine/polling_constants.h"
#include "components/sync/protocol/client_commands.pb.h"
#include "components/sync/test/fake_server/sessions_hierarchy.h"
#include "testing/gmock/include/gmock/gmock.h"

using sessions_helper::CheckInitialState;
using sessions_helper::OpenTab;
using syncer::SyncPrefs;
using testing::Gt;

namespace {

const char kURL1[] = "data:text/html,<html><title>Test</title></html>";

class TwoClientPollingSyncTest : public SyncTest {
 public:
  TwoClientPollingSyncTest() : SyncTest(TWO_CLIENT) {}
  ~TwoClientPollingSyncTest() override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TwoClientPollingSyncTest);
};

class SessionCountMatchChecker : public SingleClientStatusChangeChecker {
 public:
  SessionCountMatchChecker(int expected_count,
                           browser_sync::ProfileSyncService* service,
                           fake_server::FakeServer* fake_server)
      : SingleClientStatusChangeChecker(service),
        expected_count_(expected_count),
        verifier_(fake_server) {}

  // StatusChangeChecker implementation.
  bool IsExitConditionSatisfied() override {
    return verifier_.VerifyEntityCountByType(expected_count_, syncer::SESSIONS);
  }

  std::string GetDebugMessage() const override {
    return "Waiting for a matching number of sessions to be refleted on the "
           "fake server.";
  }

 private:
  const int expected_count_;
  fake_server::FakeServerVerifier verifier_;
};

// This test writes from one client (0) and makes sure the data arrives
// on a remote client (1) even if notifications don't work.
// Because the initial run of sync is doing a number of extra sync cycles,
// this test is structured in 2 phases. In the first phase, we simply bring
// up two clients and have them sync some data.
// In the seconed phase, we take down client 1 and while it's down upload more
// data from client 0. That second phase will rely on polling on client 1 to
// receive the update.
IN_PROC_BROWSER_TEST_F(TwoClientPollingSyncTest, ShouldPollOnStartup) {
  ASSERT_TRUE(SetupSync()) << "SetupSync() failed.";

  // Phase 1.
  ASSERT_TRUE(CheckInitialState(0));
  ASSERT_TRUE(CheckInitialState(1));
  ASSERT_TRUE(OpenTab(0, GURL(chrome::kChromeUIHistoryURL)));
  GetClient(0)->AwaitMutualSyncCycleCompletion(GetClient(1));

  // Phase 2.
  // Disconnect client 1 from sync and write another change from client 0.
  // Disconnnect the remote client from the invalidation service.
  DisableNotificationsForClient(1);
  // Make sure no extra sync cycles get triggered by test infrastructure.
  StopConfigurationRefresher();
  GetClient(1)->StopSyncService(syncer::SyncService::KEEP_DATA);

  ASSERT_TRUE(OpenTab(0, GURL(kURL1)));
  SessionCountMatchChecker server_checker(4, GetSyncService(0),
                                          GetFakeServer());
  EXPECT_TRUE(server_checker.Wait());

  // Now start up the remote client (make sure it should start a poll after
  // start-up) and verify it receives the latest changes and the poll cycle
  // updated the last-poll-time.
  // All data is already there, so we can get it in the first poll. Choose
  // larger intervals to verify the poll-on-start logic.
  SyncPrefs remote_prefs(GetProfile(1)->GetPrefs());
  remote_prefs.SetShortPollInterval(base::TimeDelta::FromMinutes(2));
  remote_prefs.SetLongPollInterval(base::TimeDelta::FromMinutes(2));
  base::Time remote_start = base::Time::Now();
  base::Time new_last_poll_time = base::Time::Now() -
                                  base::TimeDelta::FromMinutes(2) -
                                  base::TimeDelta::FromMilliseconds(100);
  remote_prefs.SetLastPollTime(new_last_poll_time);
  ASSERT_TRUE(GetClient(1)->StartSyncService()) << "SetupSync() failed.";
  GetClient(0)->AwaitMutualSyncCycleCompletion(GetClient(1));
  EXPECT_THAT(remote_prefs.GetLastPollTime(), Gt(remote_start));
}

}  // namespace