# Computer-Graphic-Program
C++实现直线，裁剪，圆内接多边形等计算机图形学算法(Cohen-Sutherland, DDA, Bresham, Seed-fill, etc)
 * Theme: 基本图形学操作
 * Description: CG期末实验
 * Author: 周财霖
 * ID: 16333279
 * Date: 2020/7/24
项目文件：Drawing
程序文件：Drawing.exe

功能：
1DDA画线，Bresham画线（注：绿色为DDA结果，蓝色为Bres结果）
2裁剪画线（注：目前仅支持对DDA得到的直线进行裁剪，Bresham直线不能完美裁剪）
3圆多边形（注1：请先画圆，如果有多个圆则仅显示最后一个圆的逼近多边形  注2：误差值已设3）
4种子填充（注：多边形已预先设置，种子点已设置为（170,170））

Feature：
1 Draw line with DDA(green),Bresham(blue) alogorithms.
2 Clip line (only DDA line applicable) with Cohen-Sutherland alogorithm
3 Draw cirle with Bresham middle poing algorithm
3 Generate Inscribed Polygon of painted circle（Approximate value set as 3）
4 Fill set polygon with Seed-fill algorithm（Seed（170,170））
