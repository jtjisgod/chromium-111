// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/suggest_internals/suggest_internals_ui.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "chrome/common/webui_url_constants.h"
#include "chrome/grit/suggest_internals_resources.h"
#include "chrome/grit/suggest_internals_resources_map.h"
#include "content/public/browser/web_ui_data_source.h"

SuggestInternalsUI::SuggestInternalsUI(content::WebUI* web_ui)
    : ui::MojoWebUIController(web_ui, /*enable_chrome_send=*/false) {
  content::WebUIDataSource* source =
      content::WebUIDataSource::Create(chrome::kChromeUISuggestInternalsHost);

  webui::SetupWebUIDataSource(source,
                              base::make_span(kSuggestInternalsResources,
                                              kSuggestInternalsResourcesSize),
                              IDR_SUGGEST_INTERNALS_SUGGEST_INTERNALS_HTML);
  webui::EnableTrustedTypesCSP(source);

  content::WebUIDataSource::Add(Profile::FromWebUI(web_ui), source);
}

SuggestInternalsUI::~SuggestInternalsUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(SuggestInternalsUI)
