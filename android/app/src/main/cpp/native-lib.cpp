//extern "C" {
//
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include "libavfilter/avfilter.h"
//#include "libavcodec/jni.h"
//
//}

#include <jni.h>
#include <string>

//#ifdef  __cplusplus
//extern "C" {
//#endif
//
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include "libavfilter/avfilter.h"
//#include "libavcodec/jni.h"
//#ifdef  __cplusplus
//};
//#endif




extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavcodec/jni.h>

JNIEXPORT jstring JNICALL
Java_com_glumes_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    char info[40000] = {0};

    av_register_all();


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

}