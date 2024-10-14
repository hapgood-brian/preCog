//------------------------------------------------------------------------------
//                  The best method for accelerating a computer
//                     is the one that boosts it by 9.8 m/s2.
//------------------------------------------------------------------------------
// Published under the GPL3 license; see LICENSE for more information.
//------------------------------------------------------------------------------

using namespace EON;
using namespace gfc;

//================================================+=============================
//Serialization:{                                 |
  //e_toSavePath:{                                |

    string e_toSavePath( const string& path ){
      const auto&_3 = path.left( 3 );
      const auto&_2 = path.left( 2 );
      if(( *path != '/' )&&(_2 != "~/" )&&(_2 != "./" )&&( _3 != "../" )){
        return e_packagePath() + path;
      }
      return path;
    }

  //}:                                            |
//}:                                              |
//================================================+=============================
