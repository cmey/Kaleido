#include "base/memory/weak_ptr.h"
#include "base/files/file_util.h"
#include "headless/public/devtools/domains/page.h"
#include "headless/public/devtools/domains/runtime.h"
#include "headless/public/headless_browser.h"
#include "headless/public/headless_devtools_client.h"
#include "headless/public/headless_devtools_target.h"
#include "headless/public/headless_web_contents.h"

#include "scopes/BaseScope.h"

#ifndef CHROMIUM_ORCA_NEXT_H
#define CHROMIUM_ORCA_NEXT_H


class Kaleido : public headless::HeadlessWebContents::Observer,
                 public headless::page::Observer,
                 public headless::runtime::Observer
{
public:
    Kaleido(headless::HeadlessBrowser* browser,
            headless::HeadlessWebContents* web_contents,
            std::string tmpFileName,
            BaseScope *scope);

    ~Kaleido() override;

    // headless::HeadlessWebContents::Observer implementation:
    void DevToolsTargetReady() override;

    // headless::page::Observer implementation:
    void OnLoadEventFired(
            const headless::page::LoadEventFiredParams& params) override;

    void OnExecutionContextCreated(const headless::runtime::ExecutionContextCreatedParams& params) override;

    void ExportNextFigure();
    void LoadNextScript();
    void OnPDFCreated(std::string responseString, std::unique_ptr<headless::page::PrintToPDFResult> result);

    void OnExportComplete(std::unique_ptr<headless::runtime::CallFunctionOnResult> result);
    void OnScriptCompileComplete(std::unique_ptr<headless::runtime::CompileScriptResult> result);
    void OnRunScriptComplete(std::unique_ptr<headless::runtime::RunScriptResult> result);

private:
    int contextId;
    std::string tmpFileName;
    std::list<std::string> remainingLocalScriptsFiles;
    BaseScope *scope;
    base::FilePath cwd;

    // The headless browser instance. Owned by the headless library. See main().
    headless::HeadlessBrowser* browser_;
    // Our tab. Owned by |browser_|.
    headless::HeadlessWebContents* web_contents_;
    // The DevTools client used to control the tab.
    std::unique_ptr<headless::HeadlessDevToolsClient> devtools_client_;
    // A helper for creating weak pointers to this class.
    // weak_factory_ MUST BE LAST PROPERTY DEFINED!
    base::WeakPtrFactory<Kaleido> weak_factory_{this};
};

namespace {
    Kaleido* g_example;
}

#endif //CHROMIUM_ORCA_NEXT_H