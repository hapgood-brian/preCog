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
    NSString* path = [[NSFileManager defaultManager] stringWithFileSystemRepresentation:pPath length:nPath ];
    NSData*   data = [NSData dataWithContentsOfFile:path];
    if( !data ){
      e_errorf( 1018171, "Couldn't load data." );
      return false;
    }
    NSString* errorString;
    id plist = [NSPropertyListSerialization propertyListFromData:data mutabilityOption:NSPropertyListImmutable format:nil errorDescription:&errorString];
    if( !plist ){
      e_errorf( 1018171, "%s in file %s\n", [errorString UTF8String], pPath );
      return false;
    }
    return true;
  }
}
