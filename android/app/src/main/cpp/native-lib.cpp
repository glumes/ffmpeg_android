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
    LOGD("JNI_OnLoad and av_jni_set_java_vm");
    av_jni_set_java_vm(vm, 0);
    // 返回jni版本
    return JNI_VERSION_1_4;
}


JNIEXPORT jstring JNICALL
Java_com_glumes_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    char info[40000] = {0};

    AVCodec* c_temp = av_codec_next(nullptr);


    while (c_temp != nullptr) {
        if (c_temp->decode != nullptr) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%s]\n", info, c_temp->name);
        break;
//        c_temp = c_temp->next;
    }

    std::string hello = "Hello from C++";
    return env->NewStringUTF(info);
}

JNIEXPORT void JNICALL
Java_com_glumes_demo_MainActivity_decodeWithPath(JNIEnv *env, jobject thiz, jstring path) {

    const char *inputPath = "/sdcard/Download/Kobe.flv";
    const char* outputPath = "/sdcard/sintel.yuv";


    FFmpegMediaCodec* mediaCodec = new FFmpegMediaCodec;
    int ret = mediaCodec->decode(inputPath,outputPath);
    LOGD("mediacodec ret is %d",ret);

}

}

