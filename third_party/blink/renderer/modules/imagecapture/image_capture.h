// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_IMAGECAPTURE_IMAGE_CAPTURE_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_IMAGECAPTURE_IMAGE_CAPTURE_H_

#include <memory>

#include "media/capture/mojom/image_capture.mojom-blink.h"
#include "third_party/blink/public/mojom/permissions/permission.mojom-blink.h"
#include "third_party/blink/renderer/core/execution_context/execution_context_lifecycle_observer.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/collection_support/heap_hash_set.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_receiver.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_remote.h"

namespace blink {

class ExceptionState;
class ImageCaptureFrameGrabber;
class MediaStreamTrack;
class MediaTrackCapabilities;
class MediaTrackConstraints;
class MediaTrackConstraintSet;
class MediaTrackSettings;
class PhotoCapabilities;
class PhotoSettings;
class ScriptPromise;
class ScriptPromiseResolver;

class MODULES_EXPORT ImageCapture final
    : public ScriptWrappable,
      public ExecutionContextLifecycleObserver,
      public mojom::blink::PermissionObserver {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static ImageCapture* Create(ExecutionContext*,
                              MediaStreamTrack*,
                              ExceptionState&);

  // |initialized_callback| is called when settings and capabilities are
  // retrieved.
  ImageCapture(ExecutionContext*,
               MediaStreamTrack*,
               bool pan_tilt_zoom_allowed,
               base::OnceClosure initialized_callback);
  ~ImageCapture() override;

  // ExecutionContextLifecycleObserver
  void ContextDestroyed() override;

  MediaStreamTrack* videoStreamTrack() const { return stream_track_.Get(); }

  ScriptPromise getPhotoCapabilities(ScriptState*);
  ScriptPromise getPhotoSettings(ScriptState*);
  ScriptPromise takePhoto(ScriptState*, const PhotoSettings*);
  ScriptPromise grabFrame(ScriptState*);

  void GetMediaTrackCapabilities(MediaTrackCapabilities*) const;
  void SetMediaTrackConstraints(ScriptPromiseResolver*,
                                const MediaTrackConstraints* constraints);
  const MediaTrackConstraintSet* GetMediaTrackConstraints() const;
  void ClearMediaTrackConstraints();
  void GetMediaTrackSettings(MediaTrackSettings*) const;

  bool HasPanTiltZoomPermissionGranted() const;

  // Called by MediaStreamTrack::clone() to get a clone with same capabilities,
  // settings, and constraints.
  ImageCapture* Clone() const;

  void Trace(Visitor*) const override;

 private:
  using PromiseResolverFunction =
      base::OnceCallback<void(ScriptPromiseResolver*)>;

  // mojom::blink::PermissionObserver implementation.
  // Called when we get an updated PTZ permission value from the browser.
  void OnPermissionStatusChange(mojom::blink::PermissionStatus) override;

  ScriptPromise GetMojoPhotoState(ScriptState*, PromiseResolverFunction);
  void OnMojoGetPhotoState(ScriptPromiseResolver*,
                           PromiseResolverFunction,
                           bool trigger_take_photo,
                           media::mojom::blink::PhotoStatePtr);
  void OnMojoSetPhotoOptions(ScriptPromiseResolver*,
                             bool trigger_take_photo,
                             bool result);
  void OnMojoTakePhoto(ScriptPromiseResolver*, media::mojom::blink::BlobPtr);

  // If getUserMedia contains either pan, tilt, or zoom constraints, the
  // corresponding settings will be set when image capture is created.
  void SetPanTiltZoomSettingsFromTrack(
      base::OnceClosure callback,
      media::mojom::blink::PhotoStatePtr photo_state);
  // Update local track settings and capabilities once pan, tilt, and zoom
  // settings have been set. |done_callback| will be called when settings and
  // capabilities are retrieved.
  void OnSetPanTiltZoomSettingsFromTrack(base::OnceClosure done_callback,
                                         bool result);
  // Update local track settings and capabilities and call
  // |initialized_callback| to indicate settings and capabilities have been
  // retrieved.
  void UpdateMediaTrackSettingsAndCapabilities(
      base::OnceClosure initialized_callback,
      media::mojom::blink::PhotoStatePtr photo_state);

  void OnServiceConnectionError();

  void ResolveWithNothing(ScriptPromiseResolver*);
  void ResolveWithPhotoSettings(ScriptPromiseResolver*);
  void ResolveWithPhotoCapabilities(ScriptPromiseResolver*);

  // Returns true if page is visible. Otherwise returns false.
  bool IsPageVisible();

  const String& SourceId() const;

  Member<MediaStreamTrack> stream_track_;
  std::unique_ptr<ImageCaptureFrameGrabber> frame_grabber_;
  HeapMojoRemote<media::mojom::blink::ImageCapture> service_;

  // Whether the user has granted permission for the user to control camera PTZ.
  mojom::blink::PermissionStatus pan_tilt_zoom_permission_;
  // The permission service, enabling us to check for the PTZ permission.
  HeapMojoRemote<mojom::blink::PermissionService> permission_service_;
  HeapMojoReceiver<mojom::blink::PermissionObserver, ImageCapture>
      permission_observer_receiver_;

  Member<MediaTrackCapabilities> capabilities_;
  Member<MediaTrackSettings> settings_;
  Member<MediaTrackConstraintSet> current_constraints_;
  Member<PhotoSettings> photo_settings_;

  Member<PhotoCapabilities> photo_capabilities_;

  HeapHashSet<Member<ScriptPromiseResolver>> service_requests_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_IMAGECAPTURE_IMAGE_CAPTURE_H_
