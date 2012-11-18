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

#include "HSSContainer.h"
#include "HSSEmptyFilter.h"
#include "HSSMultipleSelection.h"

using namespace AXR;

HSSEmptyFilter::HSSEmptyFilter(AXRController * controller)
: HSSFilter(HSSFilterTypeLast, controller)
{

}

HSSEmptyFilter::p HSSEmptyFilter::clone() const
{
    return qSharedPointerCast<HSSEmptyFilter> (this->cloneImpl());
}

HSSEmptyFilter::~HSSEmptyFilter()
{

}

AXRString HSSEmptyFilter::toString()
{
    return "Empty Filter";
}

HSSSelection::p HSSEmptyFilter::apply(HSSSelection::p scope, bool processing)
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    if (scope->isA(HSSSelectionTypeMultipleSelection))
    {
        HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(scope);
        for (HSSMultipleSelection::iterator it=multiSel->begin(); it!=multiSel->end(); ++it) {
            this->_apply(ret, *it);
        }
    }
    else if (scope->isA(HSSSelectionTypeSimpleSelection))
    {
        this->_apply(ret, qSharedPointerCast<HSSSimpleSelection>(scope));
    }
    return ret;
}

inline void HSSEmptyFilter::_apply(HSSSimpleSelection::p & ret, HSSSimpleSelection::p selection)
{
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const HSSDisplayObject::p & theDO = *it;
        const HSSContainer::p & container = HSSContainer::asContainer(*it);

        if (container->getChildren(true)->empty())
        {
            ret->add(theDO);
        }
    }
}

HSSClonable::p HSSEmptyFilter::cloneImpl() const
{
    return HSSEmptyFilter::p(new HSSEmptyFilter(*this));
}
