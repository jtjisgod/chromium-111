// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview
 * 'pointing-stick-settings' allow users to configure their pointing stick
 * settings in system settings.
 */

import '../../icons.html.js';
import '../../settings_shared.css.js';

import {I18nMixin} from 'chrome://resources/cr_elements/i18n_mixin.js';
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {routes} from '../os_route.js';
import {RouteObserverMixin} from '../route_observer_mixin.js';
import {Route} from '../router.js';

import {getInputDeviceSettingsProvider} from './input_device_mojo_interface_provider.js';
import {InputDeviceSettingsProviderInterface, PointingStick} from './input_device_settings_types.js';
import {getTemplate} from './per_device_pointing_stick.html.js';

const SettingsPerDevicePointingStickElementBase =
    RouteObserverMixin(I18nMixin(PolymerElement));

class SettingsPerDevicePointingStickElement extends
    SettingsPerDevicePointingStickElementBase {
  static get is() {
    return 'settings-per-device-pointing-stick';
  }

  static get template() {
    return getTemplate();
  }

  static get properties() {
    return {
      isDeviceSettingsSplitEnabled: {
        type: Boolean,
        value: false,
      },

      pointingSticks: {
        type: Array,
      },
    };
  }

  protected pointingSticks: PointingStick[];
  private inputDeviceSettingsProvider: InputDeviceSettingsProviderInterface =
      getInputDeviceSettingsProvider();

  constructor() {
    super();
    this.fetchConnectedPointingSticks();
  }

  override currentRouteChanged(route: Route): void {
    // Does not apply to this page.
    if (route !== routes.PER_DEVICE_POINTING_STICK) {
      return;
    }
  }

  private async fetchConnectedPointingSticks(): Promise<void> {
    this.pointingSticks = await this.inputDeviceSettingsProvider
                              .getConnectedPointingStickSettings();
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-per-device-pointing-stick': SettingsPerDevicePointingStickElement;
  }
}

customElements.define(
    SettingsPerDevicePointingStickElement.is,
    SettingsPerDevicePointingStickElement);
