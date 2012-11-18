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

#ifndef HSSSELECTION_H
#define HSSSELECTION_H

#include <QSharedPointer>
#include "AXRString.h"
#include "HSSTypeEnums.h"

namespace AXR
{
    class HSSSimpleSelection;
    class HSSMultipleSelection;

    /**
     *  @brief 
     */
    class HSSSelection
    {
    public:
        /**
         *  The shared pointer to this kind of object.
         */
        typedef QSharedPointer<HSSSelection> p;

        virtual ~HSSSelection();

        /**
         *  When logging, you often need a string representation of the selection type.
         *  @param selectionType   The selection type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString selectionTypeStringRepresentation(HSSSelectionType selectionType);

        /**
         *  Allows you to check if this selection is of the given type.
         *  @param  type    The selection type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSSelectionType type);

        /**
         *  @return The selection type of this instance.
         */
        HSSSelectionType getSelectionType();

        virtual void addSelection(p item) =0;
        virtual QSharedPointer<HSSSimpleSelection> joinAll() =0;
        virtual QSharedPointer<HSSMultipleSelection> splitAll() =0;

    protected:
        HSSSelection(HSSSelectionType type);
        HSSSelection(const HSSSelection &other);

    private:
        HSSSelectionType selectionType;
    };
}

Q_DECLARE_METATYPE(AXR::HSSSelection::p)
Q_DECLARE_METATYPE(AXR::HSSSelection::p*)

#endif