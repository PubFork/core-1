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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSLOG_H
#define HSSLOG_H

#include "HSSAction.h"

namespace AXR
{
    /**
     *  @brief This action object type logs values to the console.
     *
     *  It can either log stuff directly
     */
    class AXR_API HSSLog : public HSSAction
    {
    public:
        /**
         *  Creates a new instance of a log action object.
         */
        HSSLog(AXRController * controller);

        /**
         *  Copy constructor for HSSLog objects. Don't call directly, use clone() instead.
         */
        HSSLog(const HSSLog & orig);

        /**
         *  Clones an instance of HSSLog and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSLog.
         */
        QSharedPointer<HSSLog> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSLog();

        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual void fire();

        /**
         *  Getter for the computed value of value.
         *  @return A shared pointer to the value.
         */
        const QSharedPointer<HSSObject> getValue() const;

    private:
        void _logParserNode(QSharedPointer<HSSParserNode> parserNode) const;
        void _logObject(QSharedPointer<HSSObject> theObj) const;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
