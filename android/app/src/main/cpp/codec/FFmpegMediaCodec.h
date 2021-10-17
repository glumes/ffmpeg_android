//
// Created by glumes on 2021/10/17.
//

#ifndef ANDROID_FFMPEGMEDIACODEC_H
#define ANDROID_FFMPEGMEDIACODEC_H


#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
};

#include "logutil.h"

class FFmpegMediaCodec {

public:
    int decode(const std::string& input,const std::string& output);
};


#endif //ANDROID_FFMPEGMEDIACODEC_H
