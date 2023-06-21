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
    /*定义结构体 调用其成员函数*/
    //输入数据缓存，视音频流个数 视音频流 文件名 时长 比特率 解封装等
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    const AVCodec *pCodec;/*存储解码器信息*/
    AVFrame *pFrame, *pFrameRGB;/*存储解码器信息*/
    AVPacket *packet;/*数据包*/
    static unsigned char *out_buffer;

     /*处理图片像素数据 图片像素格式转换 图片拉伸等 */
     static struct SwsContext *img_convert_ctx;
          /*视频流*/       /*图像*/
     int videoStream, numBytes;
       /*解码*/   /*解码成功*/
     int ret, got_picture;

     avformat_network_init();//初始化FFmpeg网络模块

     pFormatCtx = avformat_alloc_context();//初始化内存

     //AVDictionary是FFmpeg的键值对存储工具，FFmpeg经常使用AVDictionary设置/读取内部参数
     AVDictionary *avdic=NULL;
     char option_key[]="rtsp_transport";
     const char tarnsportType[] = "udp"; //tcp
     av_dict_set(&avdic, option_key, tarnsportType, 0);
     char option_key2[]="stimeout";
     char option_value2[]="3000000";
     av_dict_set(&avdic, "buffer_size", "1024000", 0);                 //画质优化
     av_dict_set(&avdic,option_key2,option_value2,0);
     char url[]="rtsp://10.12.53.227:8554/";/*网络摄像头的数据*/
     /*avformat_open_input函数*/
     //参数一：指向用户提供的AVFormatContext（由avformat_alloc_context分配）的指针。
     //参数二：要打开的流的url
     //参数三：fmt如果非空，则此参数强制使用特定的输入格式。否则将自动检测格式。
     //参数四：包含AVFormatContext和demuxer私有选项的字典。返回时，此参数将被销毁并替换为包含找不到的选项
     if (avformat_open_input(&pFormatCtx, url, NULL, &avdic) != 0)    //打开多媒体并获取信息
     {
         printf("can't open the file. \n");
         return;
     }
     if(avdic != NULL)
     {
         av_dict_free(&avdic);
     }

     //获取视频流信息
     /*avformat_find_stream_info函数*/
     //参数一：媒体文件上下文。
     //参数二：字典，一些配置选项。      /*媒体句柄*/
     if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
     {
         printf("Could't find stream infomation.\n");
         return;
     }
     videoStream = -1;/*无视频流*/
     //循环查找视频中包含的流信息，直到找到视频类型的流
     /* pFormatCtx函数*/
     //unsigned int nb_streams    当前的流数量
     //AVStream **streams;  指针数组 视频流和语音流*/
     for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
     {
         if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
         {
             videoStream = i;
         }
     }
     //如果videoStream为-1 说明没有找到视频流
     if (videoStream == -1)
     {
         printf("Didn't find a video stream.\n");
         return;
     }

     //打印流信息
     //注意：最后一个参数填0，打印输入流；最后一个参数填1，打印输出流
     av_dump_format(pFormatCtx, 0, url,0);

     //查找解码器,获取指向视频流的编解码器上下文的指针
     AVCodecParameters *codecParameters = pFormatCtx->streams[videoStream]->codecpar;
     //通过解封装之后从avstream结构体里获取CodecID（指定格式流）
     pCodec = avcodec_find_decoder(codecParameters->codec_id);
     /*编码器如果等于NULL 编码器没有找到*/
     if (pCodec == NULL)
     {
         printf("Codec not found.\n");
         return;
     }

     pCodecCtx = avcodec_alloc_context3(pCodec);
     avcodec_parameters_to_context(pCodecCtx, codecParameters);
     //设置编码器参数(不同参数对视频编质量或大小的影响)
     pCodecCtx->bit_rate =0;   //初始化为0   比特率
     pCodecCtx->time_base.num=1;  //下面两行：一秒钟25帧
     pCodecCtx->time_base.den=30;
     pCodecCtx->frame_number=1;   //每包一个视频帧

     //打开解码器
     if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
     {
         printf("Could not open codec.\n");
         return;
     }

     pFrame = av_frame_alloc();    //创建  存储解码器信息*/
     pFrameRGB = av_frame_alloc(); //创建  存储解码器信息*/

                                   //解码后的h264数据转换成RGB32
     img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                  pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                         AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);


     //图像的像素格式  图像的像素宽度  图像的像素高度(计算这个格式的图片，需要多少字节来存储)
     numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height, 1);
     out_buffer = (unsigned char*) av_malloc(numBytes);
      /*瓜分分配的空间*/
      //瓜分上一步分配到的buffer.
     av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer, AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

     int y_size = pCodecCtx->width * pCodecCtx->height;
     packet = (AVPacket *) malloc(sizeof(AVPacket)); //申请一个视频帧包的大小
     av_new_packet(packet, y_size); //分配packet的数据,为packet分配一个指定大小的内存

     while (av_read_frame(pFormatCtx, packet) == 0)
     {
         //av_read_frame
         //返回流的下一帧。此函数返回存储在文件中的内容，不对有效的帧进行验证。获取存储在文件中的帧中，
         //并为每个调用返回一个。不会的省略有效帧之间的无效数据，以便给解码器最大可用于解码的信息。
         //返回0是成功，小于0则是错误，大于0则是文件末尾，所以大于等于0是返回成功
         //每解码一个视频帧，需要先调用 av_read_frame()获得一帧视频的压缩数据，然后才能对该数据进行解码
         if (packet->stream_index == videoStream)
         {
            ret = avcodec_send_packet(pCodecCtx, packet);                    //发送数据到ffmepg，放到解码队列中
            if (ret < 0)
            {
                printf("decode error.\n");
                break;
            }
            got_picture = avcodec_receive_frame(pCodecCtx, pFrame);          //将成功的解码队列中取出1个frame

            if (!got_picture)
            {
                //颜色空间转换，最后输出到out_buffer
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);//sws_scale库可以在一个函数里面同时实现：1.图像色彩空间转换；2.分辨率缩放；3.前后图像滤波处理。
                QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                ui->label->setPixmap(QPixmap::fromImage(img));
                Delay(40);
            }
          }

         //释放一个包。
         av_packet_unref(packet);
         memset(out_buffer,0,sizeof(&out_buffer));
     }
     av_free(out_buffer);
     av_free(pFrameRGB);
     avcodec_close(pCodecCtx);//关闭给定的avcodeContext并释放与之关联的所有数据
     if(NULL != pCodecCtx){
         avcodec_free_context(&pCodecCtx);
         avdic = NULL;
     }
     if(NULL != pFormatCtx){
     avformat_close_input(&pFormatCtx);//关闭打开的输入pFormatCtx。释放它和它的所有内容并设置为空。
     pFormatCtx = NULL;
     }
}
