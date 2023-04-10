#ifndef MSDK_VIDEO_DECODER_H_
#define MSDK_VIDEO_DECODER_H_

// WebRTC
#include <api/video_codecs/video_decoder.h>
#include <common_video/include/video_frame_buffer_pool.h>
#include <rtc_base/platform_thread.h>

// msdk
#include <mfx/mfxdefs.h>
#include <mfx/mfxvideo++.h>
#include <mfx/mfxvp8.h>

#include "msdk_session.h"

class MsdkVideoDecoder : public webrtc::VideoDecoder {
 public:
  MsdkVideoDecoder(std::shared_ptr<MsdkSession> session, mfxU32 codec);
  ~MsdkVideoDecoder() override;

  // MFX_CODEC_VP8
  // MFX_CODEC_VP9
  // MFX_CODEC_AVC
  // MFX_CODEC_AV1
  static bool IsSupported(std::shared_ptr<MsdkSession> session, mfxU32 codec);

  bool Configure(const Settings& settings) override;

  int32_t Decode(const webrtc::EncodedImage& input_image,
                 bool missing_frames,
                 int64_t render_time_ms) override;

  int32_t RegisterDecodeCompleteCallback(
      webrtc::DecodedImageCallback* callback) override;

  int32_t Release() override;

  const char* ImplementationName() const override;

 private:
  static std::unique_ptr<MFXVideoDECODE> CreateDecoder(
      std::shared_ptr<MsdkSession> session,
      mfxU32 codec,
      int width,
      int height,
      bool init);

  bool InitMediaSDK();
  void ReleaseMediaSDK();

  int width_ = 0;
  int height_ = 0;
  webrtc::DecodedImageCallback* decode_complete_callback_ = nullptr;
  webrtc::VideoFrameBufferPool buffer_pool_;

  mfxU32 codec_;
  std::shared_ptr<MsdkSession> session_;
  mfxFrameAllocRequest alloc_request_;
  std::unique_ptr<MFXVideoDECODE> decoder_;
  std::vector<uint8_t> surface_buffer_;
  std::vector<mfxFrameSurface1> surfaces_;
  std::vector<uint8_t> bitstream_buffer_;
  mfxBitstream bitstream_;
};

#endif  // MSDK_VIDEO_DECODER_H_