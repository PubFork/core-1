/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSPOLYGON_H
#define HSSPOLYGON_H

#include "HSSShape.h"

namespace AXR
{
    /**
     *  @brief Shape object, representing the HSS object \@polygon.
     */
    class AXR_API HSSPolygon : public HSSShape
    {
    public:
        /**
         *  Creates a new instance of a polygon shape.
         */
        HSSPolygon(AXRController * controller);

        /**
         *  Copy constructor for HSSPolygon objects. Do not call directly, use clone() instead.
         */
        HSSPolygon(const HSSPolygon & orig);

        /**
         *  Clones an instance of HSSPolygon and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSPolygon.
         */
        QSharedPointer<HSSPolygon> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSPolygon();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);
        virtual void createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<QSharedPointer<HSSParserNode> > segments);

        virtual void drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY);

        /**
         *  Getter for the actual value of the sides property.
         *  @return A HSSUnit containing the amount of sides.
         */
        unsigned int getSides();

        /**
         *  Getter for the definition object of sides.
         *  @return A shared pointer to the parser node containing the definition object of sides.
         */
        QSharedPointer<HSSParserNode> getDSides();

        /**
         *  Setter for the definition object of sides. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of sides.
         */
        void setDSides(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect sides.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void sidesChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of the angle property.
         *  @return A HSSUnit containing the amount of angle.
         */
        HSSUnit getAngle();

        /**
         *  Getter for the definition object of angle.
         *  @return A shared pointer to the parser node containing the definition object of angle.
         */
        QSharedPointer<HSSParserNode> getDAngle();

        /**
         *  Setter for the definition object of angle. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of angle.
         */
        void setDAngle(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect angle.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void angleChanged(HSSObservableProperty source, void*data);

    protected:
        //sides
        QSharedPointer<HSSParserNode> dSides;
        unsigned int sides;
        HSSObservable * observedSides;
        HSSObservableProperty observedSidesProperty;

        //angle
        QSharedPointer<HSSParserNode> dAngle;
        HSSUnit angle;
        HSSObservable * observedAngle;
        HSSObservableProperty observedAngleProperty;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        HSSUnit _evaluatePropertyValue(
                                   void(HSSPolygon::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty
                                   );
    };
}

#endif
