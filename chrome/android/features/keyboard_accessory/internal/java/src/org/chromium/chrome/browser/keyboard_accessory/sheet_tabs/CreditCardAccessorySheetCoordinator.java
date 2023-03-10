// Copyright 2019 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.keyboard_accessory.sheet_tabs;

import android.content.Context;
import android.view.ViewGroup;

import androidx.annotation.Nullable;
import androidx.annotation.VisibleForTesting;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.chrome.browser.keyboard_accessory.AccessoryAction;
import org.chromium.chrome.browser.keyboard_accessory.AccessoryTabType;
import org.chromium.chrome.browser.keyboard_accessory.R;
import org.chromium.chrome.browser.keyboard_accessory.sheet_tabs.AccessorySheetTabModel.AccessorySheetDataPiece.Type;

/**
 * This component is a tab that can be added to the ManualFillingCoordinator. This tab
 * allows selecting credit card information from a sheet below the keyboard accessory.
 */
public class CreditCardAccessorySheetCoordinator extends AccessorySheetTabCoordinator {
    private AccessorySheetTabModel mModel = new AccessorySheetTabModel();
    private final AccessorySheetTabMediator mMediator =
            new AccessorySheetTabMediator(mModel, AccessoryTabType.CREDIT_CARDS,
                    Type.CREDIT_CARD_INFO, AccessoryAction.MANAGE_CREDIT_CARDS, null);

    /**
     * Creates the credit cards tab.
     * @param context The {@link Context} containing resources like icons and layouts for this tab.
     * @param scrollListener An optional listener that will be bound to the inflated recycler view.
     */
    public CreditCardAccessorySheetCoordinator(
            Context context, @Nullable RecyclerView.OnScrollListener scrollListener) {
        super(context.getString(R.string.autofill_payment_methods),
                IconProvider.getIcon(context, R.drawable.infobar_autofill_cc),
                context.getString(R.string.credit_card_accessory_sheet_toggle),
                context.getString(R.string.credit_card_accessory_sheet_opened),
                R.layout.credit_card_accessory_sheet, AccessoryTabType.CREDIT_CARDS,
                scrollListener);
    }

    @Override
    protected AccessorySheetTabMediator getMediator() {
        return mMediator;
    }

    @Override
    public void onTabCreated(ViewGroup view) {
        super.onTabCreated(view);
        CreditCardAccessorySheetViewBinder.initializeView((RecyclerView) view, mModel);
    }

    @VisibleForTesting
    AccessorySheetTabModel getSheetDataPiecesForTesting() {
        return mModel;
    }
}
