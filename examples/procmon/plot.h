#include "xnet/base/noncopyable.h"
#include "xnet/base/Types.h"
#include <vector>
#include <stdlib.h> // ssize_t

typedef struct gdImageStruct *gdImagePtr;

class Plot : xnet::noncopyable
{
public:
    Plot(int width, int height, int totalSeconds, int samplingPeriod);
    ~Plot();
    xnet::string plotCpu(const std::vector<double> &data);

private:
    xnet::string toPng();
    // pair<shared_ptr<void*>, int> toPng();
    int getX(ssize_t x, ssize_t total) const;
    int getY(double value) const;
    void label(double maxValue);

    // gdFont is a typedef of unnamed struct, cannot be forward declared
    // wordaround suggested in http://stackoverflow.com/questions/7256436/forward-declarations-of-unnamed-struct
    struct MyGdFont;
    typedef struct MyGdFont *MyGdFontPtr;

    const int width_;
    const int height_;
    const int totalSeconds_;
    const int samplingPeriod_;
    gdImagePtr const image_;
    MyGdFontPtr const font_;
    const int fontWidth_;
    const int fontHeight_;
    const int background_;
    const int black_;
    const int gray_;
    const int blue_;

    const int kRightMargin_;
    static const int kLeftMargin_ = 5;
    static const int kMarginY_ = 5;

    const double ratioX_;
};
