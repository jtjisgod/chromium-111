// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file was generated by:
//   tools/json_schema_compiler/compiler.py.

/** @fileoverview Interface for inputMethodPrivate that can be overriden. */

/** @interface */
function InputMethodPrivate() {}

InputMethodPrivate.prototype = {
  /**
   * Gets configurations for input methods.
   * @param {function({
   *   isPhysicalKeyboardAutocorrectEnabled: boolean,
   *   isImeMenuActivated: boolean
   * }): void} callback Callback which is called with the config object.
   */
  getInputMethodConfig: function(callback) {},

  /**
   * Gets all whitelisted input methods.
   * @param {function(!Array<{
   *   id: string,
   *   name: string,
   *   indicator: string
   * }>): void} callback Callback which is called with the input method objects.
   */
  getInputMethods: function(callback) {},

  /**
   * Gets the current input method.
   * @param {function(string): void} callback Callback which is called with the
   *     current input method.
   */
  getCurrentInputMethod: function(callback) {},

  /**
   * Sets the current input method.
   * @param {string} inputMethodId The input method ID to be set as current
   *     input method.
   * @param {function(): void=} callback Callback which is called once the
   *     current input method is set. If unsuccessful $(ref:runtime.lastError)
   *     is set.
   */
  setCurrentInputMethod: function(inputMethodId, callback) {},

  /**
   * Fetches a list of all the words currently in the dictionary.
   * @param {function(!Array<string>): void} callback Callback which is called
   *     once the list of dictionary words are ready.
   */
  fetchAllDictionaryWords: function(callback) {},

  /**
   * Adds a single word to be stored in the dictionary.
   * @param {string} word A new word to add to the dictionary.
   * @param {function(): void=} callback Callback which is called once the word
   *     is added. If unsuccessful $(ref:runtime.lastError) is set.
   */
  addWordToDictionary: function(word, callback) {},

  /**
   * Gets whether the encrypt sync is enabled.
   * @param {function(boolean): void=} callback Callback which is called to
   *     provide the result.
   */
  getEncryptSyncEnabled: function(callback) {},

  /**
   * Sets the XKB layout for the given input method.
   * @param {string} xkb_name The XKB layout name.
   * @param {function(): void=} callback Callback which is called when the
   *     layout is set.
   */
  setXkbLayout: function(xkb_name, callback) {},

  /**
   * Commits the text currently being composed without moving the selected text
   * range. This is a no-op if the context is incorrect.
   * @param {{
   *   contextID: number
   * }} parameters
   * @param {function(): void=} callback Called when the operation completes.
   */
  finishComposingText: function(parameters, callback) {},

  /**
   * Sets the selection range
   * @param {{
   *   contextID: number,
   *   selectionStart: (number|undefined),
   *   selectionEnd: (number|undefined)
   * }} parameters
   * @param {function(boolean): void=} callback Called when the operation
   *     completes with a boolean indicating if the text was accepted or not.
   */
  setSelectionRange: function(parameters, callback) {},

  /**
   * Shows the input view window. If the input view window is already shown,
   * this function will do nothing.
   * @param {function(): void=} callback Called when the operation completes.
   */
  showInputView: function(callback) {},

  /**
   * Hides the input view window. If the input view window is already hidden,
   * this function will do nothing.
   * @param {function(): void=} callback Called when the operation completes.
   */
  hideInputView: function(callback) {},

  /**
   * Opens the options page for the input method extension. If the input method
   * does not have options, this function will do nothing.
   * @param {string} inputMethodId ID of the input method to open options for.
   */
  openOptionsPage: function(inputMethodId) {},

  /**
   * Gets the composition bounds
   * @param {function(!Array<{
   *   x: number,
   *   y: number,
   *   w: number,
   *   h: number
   * }>): void} callback Callback which is called to provide the result
   */
  getCompositionBounds: function(callback) {},

  /**
   * Gets the surrounding text of the current selection
   * @param {number} beforeLength The number of characters before the current
   *     selection.
   * @param {number} afterLength The number of characters after the current
   *     selection.
   * @param {function({
   *   before: string,
   *   selected: string,
   *   after: string
   * }): void} callback Callback which is called to provide the result
   */
  getSurroundingText: function(beforeLength, afterLength, callback) {},

  /**
   * Gets the current values of all settings for a particular input method
   * @param {string} engineID The ID of the engine (e.g. 'zh-t-i0-pinyin',
   *     'xkb:us::eng')
   * @param {function((!chrome.inputMethodPrivate.InputMethodSettings|undefined)): void}
   *     callback Callback to receive the settings
   */
  getSettings: function(engineID, callback) {},

  /**
   * Sets the value of all settings for a particular input method
   * @param {string} engineID The ID of the engine (e.g. 'zh-t-i0-pinyin',
   *     'xkb:us::eng')
   * @param {!chrome.inputMethodPrivate.InputMethodSettings} settings The
   *     settings to set
   * @param {function(): void=} callback Callback to notify that the new value
   *     has been set
   */
  setSettings: function(engineID, settings, callback) {},

  /**
   * Set the composition range. If this extension does not own the active IME,
   * this fails.
   * @param {{
   *   contextID: number,
   *   selectionBefore: number,
   *   selectionAfter: number,
   *   segments: (!Array<{
   *     start: number,
   *     end: number,
   *     style: !chrome.inputMethodPrivate.UnderlineStyle
   *   }>|undefined)
   * }} parameters
   * @param {function(boolean): void=} callback Called when the operation
   *     completes with a boolean indicating if the text was accepted or not. On
   *     failure, $(ref:runtime.lastError) is set.
   */
  setCompositionRange: function(parameters, callback) {},

  /**
   * Set the autocorrect range and autocorrect word. If this extension does not
   * own the active IME, this fails.
   * @param {{
   *   contextID: number,
   *   autocorrectString: string,
   *   selectionStart: number,
   *   selectionEnd: number
   * }} parameters
   * @param {function(): void=} callback Called when the operation completes. On
   *     failure, chrome.runtime.lastError is set.
   */
  setAutocorrectRange: function(parameters, callback) {},

  /**
   * Resets the current engine to its initial state. Fires an OnReset event.
   */
  reset: function() {},
};

/**
 * Fired when the input method is changed.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onChanged;

/**
 * Fired when the custom spelling dictionary is loaded.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onDictionaryLoaded;

/**
 * Fired when words are added or removed from the custom spelling dictionary.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onDictionaryChanged;

/**
 * Fired when the IME menu is activated or deactivated.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onImeMenuActivationChanged;

/**
 * Fired when the input method or the list of active input method IDs is
 * changed.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onImeMenuListChanged;

/**
 * Fired when the input.ime.setMenuItems or input.ime.updateMenuItems API is
 * called.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onImeMenuItemsChanged;

/**
 * This event is sent when focus enters a text box. It is sent to all extensions
 * that are listening to this event, and enabled by the user.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onFocus;

/**
 * This event is sent when the settings for any input method changed. It is sent
 * to all extensions that are listening to this event, and enabled by the user.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onSettingsChanged;

/**
 * This event is sent when the screen is being mirrored or the desktop is being
 * cast.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onScreenProjectionChanged;

/**
 * This event is sent when a new set of suggestions has been generated
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onSuggestionsChanged;

/**
 * This event is sent when input method options are changed.
 * @type {!ChromeEvent}
 */
InputMethodPrivate.prototype.onInputMethodOptionsChanged;
