#include <jni.h>
#include <string>

#include "common/logutil.h"

#include "codec/FFmpegMediaCodec.h"

extern "C" {

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavcodec/jni.h"



JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    av_jni_set_java_vm(vm, 0);
    return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL
Java_com_glumes_demo_MainActivity_decodeWithPath(JNIEnv *env, jobject thiz, jstring path) {

    const char *inputPath = "/sdcard/Download/Kobe.flv";
    const char* outputPath = "/sdcard/sintel.yuv";

char info[40000] = {0};

AVCodec *c_temp = av_codec_next(NULL);
while (c_temp != NULL) {
    if (c_temp->decode != NULL) {
        sprintf(info, "%s[Dec]", info);
    } else {
        sprintf(info, "%s[Enc]", info);
    }

    switch (c_temp->type) {
        case AVMEDIA_TYPE_VIDEO:
            sprintf(info, "%s[Video]", info);
            break;
        case AVMEDIA_TYPE_AUDIO:
            sprintf(info, "%s[Audio]", info);
            break;
        default:
            sprintf(info, "%s[Other]", info);
            break;
    }
    sprintf(info, "%s %10s\n", info, c_temp->name);
    c_temp = c_temp->next;
}

    LOGD("%s",info);

    FFmpegMediaCodec* mediaCodec = new FFmpegMediaCodec;
    int ret = mediaCodec->decode(inputPath,outputPath);
    LOGD("mediacodec ret is %d",ret);

}

}

