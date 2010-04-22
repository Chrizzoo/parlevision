#ifndef OPENCVIMAGE_H
#define OPENCVIMAGE_H

#include <opencv/cv.h>

#include "Types.h"

namespace plv {

class OpenCVImage : public Data
{
public:
    int getId() const { return m_id; }
    IplImage* getImage() const { return m_img; }

    OpenCVImage( int id, IplImage* img );
    OpenCVImage( const OpenCVImage& other );

    static const QString& getType();

    /** implementation of the isCompatible method of the Resource abstract
      * base class
      */
    //virtual bool isCompatibleFormat( const ComplexData* other );

protected:
    unsigned int m_id;
    IplImage* m_img;

    virtual ~OpenCVImage();
};

}

#endif