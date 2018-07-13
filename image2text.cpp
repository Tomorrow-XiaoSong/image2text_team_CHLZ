#include"image2text.h"


Image2Text::Image2Text(string input_image_address)
{
	input_image = imread(input_image_address, IMREAD_UNCHANGED);//打开方式为：IMREAD_UNCHANGED,8位，彩色或非彩色
	if (input_image.empty()) {									 //打开错误时发送错误信息
		cerr << "Error! The input image can't be read...\n";
		exit(0);
	}

}
Image2Text::~Image2Text() {
	delete[] huidu_matrix.matrix;
	delete[] char_image;
	delete[] color_matrix.matrix;
}
Image2Text::huiduMatrix Image2Text::RGB_to_huiduMatrix(Mat RGB, int width, int height) //返回值为类内声明的结构体
{

	//彩图 转 灰度图片 
	Mat huidu_image, result_image;
	cvtColor(RGB, huidu_image, COLOR_BGR2GRAY);							//直接调用cvtColor函数，通过第三个参数设置转换方式为彩图转灰度图片

																		//输出像素值的变化
	if (width == NONDESTRUCTIVE_WIDTH && huidu_image.cols < NONDESTRUCTIVE_WIDTH && huidu_image.rows < NONDESTRUCTIVE_HEIGHT) {
		//当未设置输出图片宽度宽度且图片宽高分别比NONDESTRUCTIVE_WIDTH和NONDESTRUCTIVE_HEIGHT小时，不对图片进行操作
		result_image = huidu_image.clone();
	}
	else {
		int image_width = huidu_image.cols;								//初始值为输入图像宽度，会随图片压缩处理而改变
		int image_height = huidu_image.rows;							//初始值为输入图像高度，会随图片压缩处理而改变
		if (width == NONDESTRUCTIVE_WIDTH) {							//当用户没有自定义输出图片的宽度时
			if (image_width >= NONDESTRUCTIVE_WIDTH) {					//宽度判断优先，若图片像素宽度大于NONDESTRUCTIVE_WIDTH，则以宽度为NONDESTRUCTIVE_WIDTH对图片按比例缩小
				height = image_height * (NONDESTRUCTIVE_WIDTH / ((double)image_width));
				image_height = height;
				width = NONDESTRUCTIVE_WIDTH;
			}
			if (image_height >= NONDESTRUCTIVE_HEIGHT) {				//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
																		//若图片像素高度大于NONDESTRUCTIVE_HEIGHT，则以高度为NONDESTRUCTIVE_HEIGHT对图片按比例进行缩小
				width = image_width * (NONDESTRUCTIVE_HEIGHT / ((double)image_height));
				height = NONDESTRUCTIVE_HEIGHT;
			}
		}
		resize(huidu_image, result_image, Size(width, height));			//对图片进行宽度改为width，高度改为height的变化
	}

	//灰度图片 转 灰度矩阵
	int nl = result_image.rows;								//行数  
	int nc = result_image.cols;								//列数
	huidu_matrix.matrix = new int[nl*(nc * result_image.channels()) + 1];				//定义灰度矩阵，用一维数组表示二维数组，便于后边操作
																						//矩阵列数 = 像素点列数*通道数（此处为灰度图片只有一个gray通道。彩色图片有RGB三个通道）
	huidu_matrix.width = nc;								//灰度矩阵宽度赋值
	huidu_matrix.height = nl;								//灰度矩阵高度赋值
	for (int j = 0; j<nl; j++) {
		uchar* data = result_image.ptr<uchar>(j);			//data记录图片每行像素的起始地址,ptr<uchar>(j)获得第j行的头指针，<uchar>是图片中的数据类型
		for (int i = 0; i<nc; i++) {						//将该行地址下的所有像素记录到灰度矩阵的对应行中
			huidu_matrix.matrix[j*nc + i] = data[i];
		}
	}
	return huidu_matrix;
}

Image2Text::colorMatrix Image2Text::RGB_to_colorMatrix(Mat RGB, int width, int height)
{
	int nl = RGB.rows;
	int nc = RGB.cols;
	color_matrix.matrix = new colorPx[nl*(nc * RGB.channels()) + 1];
	color_matrix.width = nc;
	color_matrix.height = nl;
	for (int j = 0; j < nl; j++) {
		uchar* color_data = RGB.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {								//图片颜色三通道排列顺序：绿，红，蓝
			color_matrix.matrix[i].Green = color_data[3 * i + 1];
			color_matrix.matrix[i].Red = color_data[3 * i + 2];
			color_matrix.matrix[i].Blue = color_data[3 * i + 3];
		}
	}
	return color_matrix;
}

char* Image2Text::huiduMatrix_to_charImage(const huiduMatrix &huiduMatrix)
{
	//未完成——把灰度矩阵转化为字符画
	cerr << "the function\"huiduMatrix_to_charImage()\" hadn't been finish...\n";
	system("pause");
	exit(0);
	return char_image;
}

char* Image2Text::huiduMatrix_to_nondestructiveImage(const huiduMatrix &huidu_matrix) {
	char huidu_char[] = "@#xo+. ";									//灰度字符
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {							//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {						//0-35:@   36-71:#   72-108:x   109-144:o   145-180:+   181-216:-   217-255: 
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 35)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 71)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 108)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 180)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else char_image[j*huidu_matrix.width + i] = huidu_char[6];
		}
	}
	return char_image;
}

void Image2Text::to_txt(string output_address, int type_of_outputImage, int width, int height) {
	char * result_image;										//记录生成的字符画矩阵
	int nl, nc;											//记录生成的字符画矩阵的宽度nc，高度nl
	switch (type_of_outputImage) {									//根据用户选择的输出图片类型：无损图片或者字符画图片来输出txt
	case NONDESTRUCTIVEIMAGE:									//txt格式：宋体 常规体 大小1  max500*500
	{
		result_image = huiduMatrix_to_nondestructiveImage(RGB_to_huiduMatrix(this->input_image));
		nl = RGB_to_huiduMatrix(this->input_image).height;
		nc = RGB_to_huiduMatrix(this->input_image).width;
		break;
	}
	case CHARIMAGE:											//txt格式：宋体 黑体 大小8  max70*70
	{
		result_image = huiduMatrix_to_charImage(RGB_to_huiduMatrix(this->input_image, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, width, height).width;
		break;
	}
	default:											//type_of_outputImage为未定义的格式时，打印错误信息
	{
		cerr << "the type of the outputImage only can be NONDESTRUCTIVEIMAGE or CHARIMAGE.\n";
		system("pause");
		exit(0);
		break;
	}
	}

	//标准化输出文件的命名格式为 xxx.txt
	if (output_address.substr(output_address.length() - 4, 4) != ".txt") {
		string a = output_address.substr(output_address.length() - 5, 4);
		output_address += ".txt";
	}

	//将生成的字符画图片储存\
	未完成——控制.txt字体格式
	ofstream f(output_address, ios::out);
	for (int j = 0; j < nl; j++) {
		for (int i = 0; i < nc; i++) {
			f << char_image[j*nc + i];
			f << char_image[j*nc + i];
		}
		f << '\n';
	}
	f.close();
}


void Image2Text::to_html(string output_address,int width, int height)
{
	char * result_image;										//记录生成的字符画矩阵
	int nl, nc;											//记录生成的字符画矩阵的宽度nc，高度nl
	result_image = huiduMatrix_to_charImage(RGB_to_huiduMatrix(this->input_image, width, height));
	nl = RGB_to_huiduMatrix(this->input_image, width, height).height;
	nc = RGB_to_huiduMatrix(this->input_image, width, height).width;
	//标准化输出文件的命名格式为 xxx.html
	if (output_address.substr(output_address.length() - 5, 5) != ".html") {
	output_address += ".html";
	}
	
	ofstream f(output_address, ios::out);
	f<<"<?xml version="1.0" encoding="UTF-8"?>\n"
	f<<"<!DOCTYPE html PUBLIC \"-\/\/W3C\/\/DTD XHTML 1.1\/\/EN\" \"http:\/\/www.w3.org\/TR\/xhtml11\/DTD\/xhtml11.dtd\">\n";
	f<<"<html xmlns=\"http:\/\/www.w3.org\/1999\/xhtml\" xml:lang=\"en\">\n";
	f<<"<head>\n";
	f<<"<title>字符画<\/title>\n";
	f<<"<meta name=\"generator\" content=\"Ascgen dotNET 2.0.0\" \/>\n";
	f<<"<style type=\"text\/css\">\n";
	f<<"<!--\n";
	f<<"#ascgen-image pre {\n";
	f<<"font-family: \"Lucida Console\", monospace;\n";
	f<<"font-size: 9pt;\n";
	f<<"background-color: #FFFFFF;\n";
	f<<"color: #000000;\n";
	f<<"float: left;\n";
	f<<"line-height: 12px;\n";
	f<<"border: 1px solid #000000;\n";
	f<<"}\"\n";
	f<<"<\/style>\n";
	f<<"<\/head>\n";
	f<<"<body>\n";
	f<<"<div id=\"ascgen-image\">\n";
 	for (int j = 0; j < nl; j++) 
	{
		f<<"<p>";
 		for (int i = 0; i < nc; i++) 
		{
 			f << char_image[j*nc + i];
 			f << char_image[j*nc + i];
 		}
		f<<"<\/p>";
 		f << '\n';
 	}
	f<<"<\/div>";
	f<<"<\/body>";
	f<<"<\/html>";
 	f.close();//输出html
	cerr << "the function hadn't\"to_html\" been finish...\n";
	system("pause");
	exit(0);
};
