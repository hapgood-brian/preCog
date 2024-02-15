#import<Foundation/Foundation.h>

using namespace EON;
using namespace gfc;

//------------------------------------------------------------------------------
// Verify PBX formatting and data. This code serializes in the pbxproj and makes
// sure it's up to snuff.
//------------------------------------------------------------------------------

bool verifyPBX( const gfc::string& path ){
  @autoreleasepool{
    ccp pPath = path.c_str();
    u64 nPath = path.len();
    NSString* path = [[NSFileManager defaultManager]
      stringWithFileSystemRepresentation:pPath
      length:nPath ];
    NSData* data = [NSData dataWithContentsOfFile:path];
    if( !data ){
      e_break( "Couldn't load data." );
      return false;
    }
    NSError* err = nil;
    id plist = [NSPropertyListSerialization
      propertyListWithData:data
      options:NSPropertyListImmutable
      format:nil
      error:&err];
    if( !plist ){
      e_break( e_xfs(
        "%s in file %s\n"
        , [err.localizedDescription UTF8String]
        , pPath ));
      return false;
    }
    return true;
  }
}
