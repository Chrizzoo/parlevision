/**
  * Copyright (C)2010 by Michel Jansen and Richard Loos
  * All rights reserved.
  *
  * This file is part of the plvcore module of ParleVision.
  *
  * ParleVision is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * ParleVision is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * A copy of the GNU General Public License can be found in the root
  * of this software package directory in the file LICENSE.LGPL.
  * If not, see <http://www.gnu.org/licenses/>.
  */

#include <QDebug>

#include "EdgeDetectorLaplace.h"
#include <plvcore/OpenCVImage.h>
#include <plvcore/OpenCVImagePin.h>
#include <plvcore/Util.h>

using namespace plv;
using namespace plvopencv;

EdgeDetectorLaplace::EdgeDetectorLaplace() :
        m_apertureSize(3)
{
    m_inputPin = createOpenCVImageInputPin( "input", this );
    m_outputPin = createOpenCVImageOutputPin( "output", this );

    // formats directly supported by cvLaplace function
    m_inputPin->addSupportedDepth( IPL_DEPTH_8S );
    m_inputPin->addSupportedDepth( IPL_DEPTH_8U );
    m_inputPin->addSupportedDepth( IPL_DEPTH_32F );
    m_inputPin->addSupportedChannels( 1 );

    // function outputs 16 bit image when input is 8-bit
    // and 32F when input is 32-bit float
    m_outputPin->addSupportedDepth( IPL_DEPTH_16S );
    m_outputPin->addSupportedDepth( IPL_DEPTH_32F );
    m_outputPin->addSupportedChannels( 1 );
}

EdgeDetectorLaplace::~EdgeDetectorLaplace()
{
}

void EdgeDetectorLaplace::init()
{
}

void EdgeDetectorLaplace::deinit() throw ()
{
}

void EdgeDetectorLaplace::start()
{
}

void EdgeDetectorLaplace::stop()
{
}

void EdgeDetectorLaplace::process()
{
    RefPtr<OpenCVImage> srcPtr = m_inputPin->get();

    // destination image should be at least 16 bits to avoid overflow
    // see e.g. http://opencv.willowgarage.com/documentation/image_filtering.html?highlight=cvsobel#cvLaplace
    OpenCVImageProperties props = srcPtr->getProperties();
    if( props.getDepth() != IPL_DEPTH_32F )
    {
        props.setDepth( IPL_DEPTH_16S );
    }
    RefPtr<OpenCVImage> dstPtr = OpenCVImageFactory::get( props );

    // open for reading
    const IplImage* src = srcPtr->getImage();

    // open image for writing
    IplImage* dst = dstPtr->getImageForWriting();

    assert( m_apertureSize == 1 || m_apertureSize == 3 || m_apertureSize == 5 || m_apertureSize == 7 );

    // do laplace operation
    cvLaplace( src, dst, m_apertureSize );

    // publish output
    m_outputPin->put( dstPtr );
}

void EdgeDetectorLaplace::setApertureSize(int i)
{
    QMutexLocker lock( m_propertyMutex );

    //aperture size must be odd and positive,
    //min 1 and max 7
    if (i < 1)
        i = 1;
    else if (i > 7)
        i = 7;
    else if( isEven(i) )
    {   //even: determine appropriate new odd value
        //we were increasing -- increase to next odd value
        if( i > m_apertureSize )
            i++;
        //we were decreasing -- decrease to next odd value
        else
            i--;
    }
    m_apertureSize = i;
    emit(apertureSizeChanged(i));
}
