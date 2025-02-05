/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_WyciwygChannelParent_h
#define mozilla_net_WyciwygChannelParent_h

#include "mozilla/net/PWyciwygChannelParent.h"
#include "mozilla/net/NeckoCommon.h"
#include "nsIStreamListener.h"

#include "nsIWyciwygChannel.h"
#include "nsIInterfaceRequestor.h"
#include "nsILoadContext.h"

namespace mozilla {
namespace dom {
class PBrowserParent;
}  // namespace dom

namespace net {

class WyciwygChannelParent : public PWyciwygChannelParent,
                             public nsIStreamListener,
                             public nsIInterfaceRequestor {
  friend class PWyciwygChannelParent;

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIREQUESTOBSERVER
  NS_DECL_NSISTREAMLISTENER
  NS_DECL_NSIINTERFACEREQUESTOR

  WyciwygChannelParent();

 protected:
  virtual ~WyciwygChannelParent() = default;

  mozilla::ipc::IPCResult RecvInit(
      const URIParams& uri, const ipc::PrincipalInfo& aRequestingPrincipalInfo,
      const ipc::PrincipalInfo& aTriggeringPrincipalInfo,
      const ipc::PrincipalInfo& aPrincipalToInheritInfo,
      const uint32_t& aSecurityFlags, const uint32_t& aContentPolicyType);
  mozilla::ipc::IPCResult RecvAsyncOpen(
      const URIParams& original, const uint32_t& loadFlags,
      const IPC::SerializedLoadContext& loadContext,
      const PBrowserOrId& parent);
  mozilla::ipc::IPCResult RecvWriteToCacheEntry(
      const nsDependentSubstring& data);
  mozilla::ipc::IPCResult RecvCloseCacheEntry(const nsresult& reason);
  mozilla::ipc::IPCResult RecvSetCharsetAndSource(const int32_t& source,
                                                  const nsCString& charset);
  mozilla::ipc::IPCResult RecvSetSecurityInfo(const nsCString& securityInfo);
  mozilla::ipc::IPCResult RecvCancel(const nsresult& statusCode);
  mozilla::ipc::IPCResult RecvAppData(
      const IPC::SerializedLoadContext& loadContext,
      const PBrowserOrId& parent);

  virtual void ActorDestroy(ActorDestroyReason why) override;

  bool SetupAppData(const IPC::SerializedLoadContext& loadContext,
                    const PBrowserOrId& aParent);

  nsCOMPtr<nsIWyciwygChannel> mChannel;
  bool mIPCClosed;
  bool mReceivedAppData;
  nsCOMPtr<nsILoadContext> mLoadContext;
};

}  // namespace net
}  // namespace mozilla

#endif  // mozilla_net_WyciwygChannelParent_h
