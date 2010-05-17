#include "StdAfx.h"
#include "Check.h"
#include "String.h"

//所有的以新风格命名的函数都在 cv 命名空间中
//如果希望不要每次都输入 cv:: ，则可使用下面语句
//using namespace cv;

CCheck::CCheck(void)
{
}

CCheck::~CCheck(void)
{
}


//初始化图像
void CCheck::Init()
{
	m_ReadImage1 = NULL;
	m_ReadImage2 = NULL;
	m_ReadImage2 = NULL;

}


bool CCheck::Load(CString strPath)
{
	if(strPath.IsEmpty())
	{
		return false;
	}
	IplImage *image;
	image = cvLoadImage( strPath, 0);
	if(!image)
		return false;
//	SetImage(image);
	cvReleaseImage(&image);
	return true;
}
/*
bool CCheck::SetImage( IplImage* image, int maxWidth, int maxHeight )
{
	if(!image)
		return false;

	//设置ReadImage
	if(!SetReadImage(image))
		return false;
	//设置GrayImage
	if(!SetGrayImage())
		return false;
	//设置BinaryImage
	if(!SetBinaryImage())
		return false;

	return true;
}
*/

//降序的冒泡排序
void CCheck::BubbleSort(DataField *a,int n)
{
	int i,j,flag=1;
	DataField temp;
    
	for(i=1;i<n&&flag==1;i++)
	{
		flag=0;
		for(j=0;j<n-i;j++)
		{	
			if(a[j].value<a[j+1].value)
			{
				flag=1;
				temp=a[j];						
				a[j]=a[j+1];		
				a[j+1]=temp;			
			}	
		}
	}	
}

DataField CCheck:: Data(CString name,int num) 
{ 
 
    const char* imagename = name;
 
	IplImage * img =cvLoadImage(imagename,0);		//以单通道的格式读取图像
    IplImage * img2=0;
	
	int height,width,step,channels; //记录图像长、宽、行长度、信道信息的变量
	uchar *data1,*data2;	//记录图像数据的数组	 
	int i,j,l,m,p=0,q=0,x,y;	//用于遍历图像时，for循环的变量
	double sum=0,temp;		//建立数据场，运算势值的临时变量
	DataField result;		//用以保存一幅图像运算后，若干个局部极值点投影成一个点
	result.value=0;
	result.x=0;
	result.y=0;
	result.number=num;
	int sum_x=0,sum_y=0;
	
	//如果打开图像出错，则输出提示信息，退出函数
    if(!img) 
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return	result;
		
	}
 
	//获取图像信息
	height=img->height;
	width=img->width;
	step=img->widthStep;
	channels=img->nChannels;
	data1=(uchar * )img->imageData;

	//用DataField类型的数组，存放图像中各个像素点，计算后的势值及其坐标
	//gettemp用以存放，图像创建为数据场后，场中各点的值及坐标
	DataField * gettemp=new DataField[height*width]; 
	//local_max_point用以存放，数据场中的局部极大值及其坐标
	DataField * local_max_point=new DataField[(height)*(width)];
	
	//对gettemp和local_max_point进行初始化
	for(i=0;i<(height)*(width);i++)
	{
		gettemp[i].number=num;
		gettemp[i].value=0;	
		local_max_point[i].value=0;
		local_max_point[i].x=0;
		local_max_point[i].y=0;
	}

    //图像像素按行归一到[0,1]
	double * picture_mat=new double [height*width];
	for(y=0;y<height;y++)
	{
		double element=0;	//图像数据每列平方求和
		for(x=0;x<width;x++)
		{
			element+=data1[y*step+x]*data1[y*step+x];
		}
		for(x=0;x<width;x++)
		{
			picture_mat[y*step+x]=data1[y*step+x]/sqrt(element);
		}
	}

	//复制图像
	img2=cvCreateImage(cvGetSize(img),img->depth,0);
	data2=(uchar *)img2->imageData;
	cvCopy(img,img2,0);

   //遍历图像，创建数据场,sigema*sigema=10
	//外面两个for循环用以遍历整张图片
	for(y=8;y<height-8;y++)		
	{
		for(x=8;x<width-8;x++)
		{
			//以(x,y)为中心点，在17*17的区域中，求出各点在中心点处产生的势值
			for(l=y-8;l<y+8;l++)
			{
				for(m=x-8;m<x+8;m++)
				{				
					//(l-y)*(l-y)+(m-x)*(m-x)为当前点到中心点的距离的平方
					temp=-(((l-y)*(l-y)+(m-x)*(m-x))/10);		//e的指数					
					gettemp[y*step+x].value+=picture_mat[l*step+m]*exp(temp);	//数据场中(x,y)处的势值
					gettemp[y*step+x].x=x;	//数据场中该点x坐标
					gettemp[y*step+x].y=y;	//数据场中该点y坐标				
				}	
			}
		}
	}
	
    //local_temp为求解局部极值点的临时变量
	DataField * local_temp=new DataField[(height-40)*(width-40)];
	i=0;
	q=0;
	//数据场中需要求解局部极值的区域的点，存进local_temp中
	for(y=20;y<height-20;y++)
		for(x=20;x<width-20;x++)
			local_temp[i++]=gettemp[y*step+x];
					
	while(q<20)
	{
		BubbleSort(local_temp,i);
		//通过排序，找出区域中第一个最大的势值点
		local_max_point[q]=local_temp[0];
		int y1=local_max_point[q].y,x1=local_max_point[q].x;
		double max_index=local_max_point[q].value;
		//消除前一个极值点对整个区域的影响
		for(j=0;j<(height-40)*(width-40);j++)
		{
			int y2=local_temp[j].y,x2=local_temp[j].x;
			temp=-(((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1))/10);
			//每个点分别减去高斯比例函数
			local_temp[j].value=local_temp[j].value-max_index*exp(temp);
		}
		q++;
	}
	delete local_temp;
	printf("\n%d\n%d\n",i,q);

	//对局部极值点投影成一个点，即进行坐标的融合
	for(i=0;i<q;i++)
	{
		if(local_max_point[i].value!=0)
		{
			//局部极值点当前坐标值的横坐标分量x*当前极值点的势值，累加到sum_x中
			sum_x+=local_max_point[i].x*local_max_point[i].value;	
			//局部极值点当前坐标值的横坐标分量x*当前极值点的势值，累加到sum_y中
			sum_y+=local_max_point[i].y*local_max_point[i].value;
			//对各个极值点的势值进行累加
			sum+=local_max_point[i].value;
		}
	}
	//result.value为各个局部极值点的平均势值
	result.value=sum/q;
	//result.x为20个局部极值点投影为一个点的x坐标
	result.x=sum_x/sum;
	//result.x为20个局部极值点投影为一个点的y坐标
	result.y=sum_y/sum;

	printf("%f\t%d%d",result.value,result.x,result.y);
	printf("\n");	

//	cvSaveImage(name,img2);
	
//	cv::namedWindow("image", CV_WINDOW_AUTOSIZE); //创建窗口
//namedWindow("image2", CV_WINDOW_AUTOSIZE); //创建窗口
//	imshow("image", img); //显示图像
//	imshow("image2", img2); //显示图像
  
	delete gettemp;
	delete local_max_point;
//	cvDestroyWindow( "image" );//销毁窗口
    cvReleaseImage( &img ); //释放图像
//	cvDestroyWindow( "image2" );//销毁窗口
    cvReleaseImage( &img2 ); //释放图像
	return result;
}

bool CCheck::SecondData(DataField *a,DataField tempcheck)//a为样本对象，tempcheck为待测对象
{
	DataField temp[8];			//用以投影在二次数据场的数据点的坐标及势值
	DataField unsort[8];		//在二次数据场中，待排序的8个对象，前7个样本对象，最后1个为待测对象
	for(int i=0;i<8;i++)
	{	
		temp[i].value=0;
		temp[i].x=0;
		temp[i].y=0;
	}
	for(int i=0;i<7;i++)
	{
		temp[i]=a[i];
	}

	temp[7]=tempcheck;
//	m_objCheck.temp1[7]=temp[7];

	double second_datafield [128] [128];	//定义128*128的二维数组，作为二次数据场
	//对二次数据场进行初始化，使其值等于0
	for(int i=0;i<128;i++)
		for(int j=0;j<128;j++)
			second_datafield[i][j]=0;

	//遍历二次数据场，求出每个点的势值
	for(int y=0;y<128;y++)
		for(int x=0;x<128;x++)
			for(int k=0;k<8;k++)
			{
				//d为二次数据场中数据点点（每幅图像）到数据场中当前点的距离的平方,假设sigema=3
				double d=-((temp[k].x-x)*(temp[k].x-x)+(temp[k].y-y)*(temp[k].y-y))/9;	//距离的平方							
				second_datafield[x][y]+=temp[k].value*exp(d);
			}
	printf("\n\n\n");
     
	//对投影到二次数据场中的8个对象进行排序
	for(int i=0;i<8;i++) 
	{	
		unsort[i].number=temp[i].number;
		unsort[i].x=temp[i].x;
		unsort[i].y=temp[i].y;
		unsort[i].value=second_datafield[temp[i].x][temp[i].y];
	}
	BubbleSort(unsort,8);

	//flag为离群点标志位，若待测对象为离群点，flag置true；否则为false
	bool flag=false;
	//若排序后，待测对象仍排在数组的末尾，则说明该对象在二次数据场中势值最小，且为离群点
	for(int i=0;i<8;i++)
	{
		if(unsort[i].number==8&&i==7&&(unsort[6].value-unsort[7].value)<2)
			flag=true;
	}
	return flag;
//	for(int i=0;i<8;i++)
//		printf("%f\t%s%d%s%d%s\t%d\n",unsort[i].value,"(",unsort[i].x,",",unsort[i].y,")",unsort[i].number);


}