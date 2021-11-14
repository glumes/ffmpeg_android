//
// Created by glumes on 2021/10/17.
//

#ifndef ANDROID_FFMPEGMEDIACODEC_H
#define ANDROID_FFMPEGMEDIACODEC_H


#include <string>
#include <jni.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libavcodec/mediacodec.h>
};

#include "logutil.h"

class FFmpegMediaCodec {

public:
    int decode(const std::string& input,const std::string& output);
    int decode(const std::string& input,jobject surface);

private:
    int decode_write(AVCodecContext *avctx, AVPacket *packet);
    int decode_render(AVCodecContext *avctx, AVPacket *packet);
};


#endif //ANDROID_FFMPEGMEDIACODEC_H
