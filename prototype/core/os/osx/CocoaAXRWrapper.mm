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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
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

#import <Cocoa/Cocoa.h>
#import "AXR.h"
#import "AXRDebugging.h"
#import "AXRError.h"
#import "CocoaAxrWrapper.h"

using namespace AXR;

CocoaAxrWrapper::CocoaAxrWrapper(AXRView * mainView)
: AXRWrapper()
{
    this->mainView = mainView;
}

AXRWrapper * CocoaAxrWrapper::createWrapper()
{
    return new CocoaAxrWrapper(NULL);
}

CocoaAxrWrapper::~CocoaAxrWrapper()
{
    
}

//throws AXRError::p
AXRFile::p CocoaAxrWrapper::getFile(std::string url)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    
    if(url.substr(0, 7) == "file://"){
        std::string clean_path = url.substr(7, url.size());
        int slashpos = clean_path.rfind("/");
        ret->setFileName(clean_path.substr(slashpos+1, clean_path.size()));
        ret->setBasePath(clean_path.substr(0, slashpos));
        
        ret->setBufferSize(20240);
        ret->setBuffer(new char[ret->getBufferSize()]);
        ret->setFileHandle(fopen(clean_path.c_str(), "r"));
        if( ret->getFileHandle() == NULL ){
            throw AXRError::p(new AXRError("CocoaAxrWrapper", "the file "+url+" doesn't exist"));
        } else if( ferror(ret->getFileHandle()) ){
            throw AXRError::p(new AXRError("CocoaAxrWrapper", "the file "+url+" couldn't be read"));
        }
        
    } else {
        std_log("http is not implemented yet");
    }
    
    return ret;
}

size_t CocoaAxrWrapper::readFile(AXRFile::p theFile)
{
    char * buffer = theFile->getBuffer();
    size_t size = fread(buffer, sizeof(buffer[0]), theFile->getBufferSize(), theFile->getFileHandle());
    if(feof(theFile->getFileHandle())){
        theFile->setAtEndOfFile(true);
    }
    if (ferror(theFile->getFileHandle())) {
        AXRError::p(new AXRError("CocoaAxrWrapper", "The file "+theFile->getFileName()+" couldn't be read"))->raise();
        return -1;
    }
    return size;
}

void CocoaAxrWrapper::closeFile(AXRFile::p theFile)
{
    fclose(theFile->getFileHandle());
    theFile->setFileHandle(NULL);
}

void CocoaAxrWrapper::handleError(AXRError::p theError)
{
    std_log(theError->toString());
}


bool CocoaAxrWrapper::openFileDialog(std::string &filePath)
{
    axr_log(AXR_DEBUG_CH_GENERAL, "CocoaAxrWrapper: Opening File Dialog");
    
    //load a file
	NSArray *fileTypes = [NSArray arrayWithObjects: @"xml", @"hss", nil];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:fileTypes];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath_s = [[openPanel filenames] objectAtIndex:0];
            filePath = std::string([filepath_s UTF8String]);
            axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "CocoaAxrWrapper: User selected file "+filePath);
            
            return true;
        }
    }
    
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "CocoaAxrWrapper: No file selected from open dialog, returning false");
    
    return false;
}

void CocoaAxrWrapper::setNeedsDisplay(bool newValue)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];                           
    [this->mainView setNeedsDisplay:newValue];
    [pool release];
}

std::string CocoaAxrWrapper::getPathToResources()
{
    std::string resourcesPath = [[[NSBundle mainBundle] resourcePath] UTF8String];
    return resourcesPath;
}