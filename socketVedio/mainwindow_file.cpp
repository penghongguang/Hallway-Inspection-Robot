#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QtCore/QDateTime"

// 调用FFmpeg的头文件
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavutil/error.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void Delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_pushButton_clicked()
{
    AVFormatContext    *pFormatCtx;
    int                videoindex;
    AVCodecContext *pCodecCtx;
    const AVCodec            *pCodec;
    AVFrame    *pFrame, *pFrameRGB;
    unsigned char *out_buffer;
    AVPacket *packet;
    int ret;
    struct SwsContext *img_convert_ctx;

    char filePath[] = "C:\\Users\\px\\Desktop\\t.mp4";

    avformat_network_init(); //初始化网络模块

    pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, filePath, NULL, NULL) != 0)
    {
        printf("Couldn't open input stream.\n");
        return ;
    }

    //获取音视频流数据信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
        printf("Couldn't find stream information.\n");
        return ;
    }
    videoindex = -1;

    //nb_streams视音频流的个数，这里当查找到视频流时就中断了。
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoindex = i;
            break;
    }
    if (videoindex == -1){
        printf("Didn't find a video stream.\n");
        return ;
    }

    //获取视频流编码结构
    AVCodecParameters *codecParameters = pFormatCtx->streams[videoindex]->codecpar;
    //查找解码器
    pCodec = avcodec_find_decoder(codecParameters->codec_id);
    if (pCodec == NULL){
        printf("Codec not found.\n");
        return ;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, codecParameters);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
        printf("Could not open codec.\n");
        return ;
    }

    //创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    //创建动态内存,创建存储图像数据的空间
    //av_image_get_buffer_size获取一帧图像需要的大小
    out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer,
        AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

    packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx, 0, filePath, 0);//此函数打印输入或输出的详细信息
    printf("-------------------------------------------------\n");

    //初始化img_convert_ctx结构
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    //av_read_frame读取一帧未解码的数据

    while (av_read_frame(pFormatCtx, packet) >= 0){
        //如果是视频数据
        if (packet->stream_index == videoindex){
            //解码一帧视频数据
            if (avcodec_send_packet(pCodecCtx, packet) < 0) {
                break ;
            }
            ret = avcodec_receive_frame(pCodecCtx, pFrame);
            if (ret < 0){
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_make_error_string(errbuf, AV_ERROR_MAX_STRING_SIZE, ret);
                printf("Error while decoding frame: %s\n", errbuf);
                break ;
            }

            sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                pFrameRGB->data, pFrameRGB->linesize);
            QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
            ui->label->setPixmap(QPixmap::fromImage(img));
            Delay(40);
        }
        av_packet_unref(packet);
    }
    sws_freeContext(img_convert_ctx);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
