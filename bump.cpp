#include "opencv/cv.hpp"
#include "opencv/highgui.h"
#include <iostream>
#include <algorithm>

//허프라인 위쪽 없애기.

using namespace cv;
using namespace std;

#define PI 3.1415926
#define Yellow_FROM 15, 30, 30
#define Yellow_TO 30, 255, 250
#define ROI 140, 140, 300,100
#define Canny_threshold 50, 120



int main()
{
    Mat src, canny, hsvImg,color_dst,canny_yellow,org;
    Mat detectYellow;
    Rect roi(ROI); //ROI

    src = imread("/Users/NAMSOO/Documents/Xcode/OpenCV/Smartcar/Smartcar/3.jpg");
    org = src.clone();
    int count1=0;
    int count2=0;
    int max=0;
    
    
    
    // Detect Yellow color
    cvtColor(src, hsvImg, CV_BGR2HSV);
    Scalar from(Yellow_FROM); //define. //get yellow color
    Scalar to(Yellow_TO);
    inRange(hsvImg, from, to, detectYellow);
    
    //morphological closing (fill small holes in the foreground)
    dilate(detectYellow, detectYellow, getStructuringElement(MORPH_RECT, Size(5, 5)));
    erode(detectYellow, detectYellow, getStructuringElement(MORPH_RECT, Size(5, 5)));
    
    //convert yellow color dectected to canny.
    Canny(detectYellow, canny_yellow, Canny_threshold);
    dilate(canny_yellow, canny_yellow, getStructuringElement(MORPH_RECT, Size(5, 5)));
    erode(canny_yellow,canny_yellow, getStructuringElement(MORPH_RECT, Size(5, 5)));
    
    // canny threshold
    Canny(src, canny, Canny_threshold);
    cvtColor(canny, color_dst, CV_GRAY2BGR );
    dilate(color_dst,color_dst, getStructuringElement(MORPH_RECT, Size(5, 5)));
    erode(color_dst,color_dst, getStructuringElement(MORPH_RECT, Size(5, 5)));
    dilate(color_dst,color_dst, getStructuringElement(MORPH_RECT, Size(5, 5)));
    erode(color_dst,color_dst, getStructuringElement(MORPH_RECT, Size(5, 5)));
    

    
    //--------
   
    vector<Vec4i> lines;
    //HoughLinesP(canny_yellow, lines, 1, CV_PI/180, 70, 50, 200 ); //original
    HoughLinesP(canny_yellow, lines, 1, CV_PI/180, 100, 50, 200 );
  
   
    for( size_t i = 0; i < lines.size(); i++ ) //가까이에 있는 방지턱 검출
    {
        
        
        Point p1,p2;
        p1 = Point(lines[i][0],lines[i][1]); //position of points.
        p2 = Point(lines[i][2], lines[i][3]);
        
        
        float angle = atan2(p1.y-p2.y,p1.x-p2.x);
        angle = abs(angle *180 / PI); //angle of 0~180
        cout<<angle;
        cout<<endl;
        
        
        
        if(angle>177 || angle<3 ) //draw virticle line
        {
           // line( color_dst, Point(lines[i][0], lines[i][1]),
              //   Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
            //line( src, Point(lines[i][0], lines[i][1]),
            //   Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 ); //src에 빨간 라인 그리기.
            
            
            line( src, Point(lines[i][0], lines[i][1]),
                 Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        
           max= std::__1::max(max, lines[i][1]);

            /*
            for( size_t j = 0; j < lines.size(); j++ )
            {
                if( max <= lines[j][1] +15)
                    
                    count1++;
            }
             */
           // linecount ++;

            
        }
           }
    
   // cout<<"number of verticle lines : "<<linecount;
    //ROI setting. to detect far speed bump. 멀리 있는 방지턱을 검출하기 위해 색깔정보가 아닌 캐니로 detect.
    Mat img_roi = canny(roi); //ROI
    vector<Vec4i> lines2;
    HoughLinesP(img_roi, lines2, 1, CV_PI/180, 50, 50, 5 );
    
    /*
    Point p3,p4;
    p3 = Point(lines2[0][0],lines2[0][1]); //position of points.
    p4 = Point(lines2[0][2], lines2[0][3]);
    
   // int crit = lines2[0][1]; //첫 직선의 y좌표.
    
     
    cout<<endl;
    cout<< p3 << endl << p4 <<endl;
*/
    
     for( size_t i = 0; i < lines2.size(); i++ )
     {
          Point p3,p4;
         p3 = Point(lines2[i][0],lines2[i][1]); //position of points.
         p4 = Point(lines2[i][2], lines2[i][3]);
        
         float angle2 = atan2(p3.y-p4.y,p3.x-p4.x);
         angle2 = abs(angle2 *180 / PI); //angle of 0~180
         cout<<angle2;
         cout<<endl;
         cout<< p3 << endl << p4 <<endl;
         
        

          if( angle2>177 || angle2<3)
           {
              line(src, Point(140+lines2[i][0], 140+lines2[i][1]),
                    Point(140+lines2[i][2], 140+lines2[i][3]), Scalar(255,0,0), 2, 8 ); //draw line in ROI in canny 초록색 라인그리기.
               //ROI를 설정하면 그 선을 그릴 때는 ROI 처음의 x,y 값만큼을 line 함수 각각의 x,y에 더해줘야 한다.
              // Rect roi(140, 140, 300,100 ); //ROI
              // line(src, Point(lines2[i][0],lines2[i][1]),
                 //   Point(lines2[i][2],lines2[i][3]), Scalar(0,0,255), 4, 8 );
               
               max= std::__1::max(max, lines2[i][1]);
               
               
               for( size_t j = 0; j < lines2.size(); j++ )
               {
                   if( max <= (lines2[j][1] +10))
                       count2++;
               }

               

           }
         
        
     }
    //cout << count<<"hello"<<endl;
    cout << max <<endl ;
    cout <<count1<<endl;
    cout << count2<<endl;
    
    
    //y값은 가장 큰 것을 기준점으로 한다. 180도 안에 들어간 것만 잡는다.
    
    rectangle(src, roi, Scalar(0,255,0), 1, 8, 0 ); //draw ROI rectangle
    imshow("src", src);
    imshow("canny", canny);
    imshow("detect yellow", detectYellow);
    //namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", color_dst );
    imshow( "canny_yellow",canny_yellow);
   imshow("original",org);
    imshow("roi",img_roi);
    
    waitKey(0);
    return 0;
}
