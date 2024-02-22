# preCog -- A AAA project generator.

Precog works for Ninja, Emscripten, Xcode 11-15, Gradle, and Visual Studio 2019-2022, in a way you can actually ship.

Works by diving into your project's directory structure, driven by a simplistic Lua script. From this static or shared libraries (.a, .lib, .dylib, .DLL, etc) are produced for a variety of platforms. Inspired by cmake and premake.

## INSTALLATION

To install preCog on your system choose the installer for your platform, found
in releases in GitHub. For macOS, this will be a .pkg. For Windows, a .msi,
and eventually for the Linux package manager of your choice.

## OVERVIEW

The preCog tool is a free build system from creepydoll.games. It supports the Xcode PBX format version 12 right out of the box and fills in the gaps where Cmake falls down on macOS. A huge amount of effort went into supporting the Xcode project format. With it, you can create/code-generate project files and an overarching `.xcworkspace` that includes them. No more 3.x format garbage that Cmake pukes out.

But the goodies don't end there! If you're on Windows you can generate projects for Visual Studio 2019 through 2022, on Linux you can generate for Ninja, for Android a `gradle.build` file, for iOS the previously mentioned Xcode project (side-by-side targets are planned for v1.0.1), and for Emscripten a WASM project.

You are free to create a single monolithic Lua script that runs for all IDEs on their various platforms (`cogfile.lua`) or break the script up into multiple `cogfile-xxx.lua` scripts with the following code:

```
--------------------------------------------------------------------------------
-- Print out what we're generating for.
--------------------------------------------------------------------------------

print('Generating for '..platform.name()..'.')

--------------------------------------------------------------------------------
-- Now include the appropriate target script based on the command line.
--------------------------------------------------------------------------------

require('src/cogfiles/cogfile.'..platform.name()..'.lua')
```

### Lua

The developer (me, Brinton Sadbad--a humorous name given to me long ago (the real name is Brian Hapgood) decided at the beginning of this process (when Cmake totally cheesed him off on account of its HORRIBLE support for Xcode) that preCog would forever keep abreast of the latest version of Lua (as of this writing that would be 5.4.6) of the Lua language. There are some really huge advantages to this decision because Lua is easy to use and just works. I was naughty, I mean, REALLY naughty, so much so that the Lua community will have all kinds of nasty things to say about me, I'm sure, by adding a C preprocessor to every `.lua` you `require()`. So just as there are different functions in Lua for loading a `chunk` there's an additional `#include<path[.lua]>` that inserts the named Lua source file directly into the outer one. In other words, exactly like C/C++. Nasty, truly ugly, but powerful as hell, because the preprocessor will not create a chunk, unlike `require` and `loadfile` do.

Every other feature of the language is supported, although I do set the "up" value of every loaded script via `require` and company. This sandboxes every aforementioned Lua chunk in the `__sandbox` table. Everything is exposed there, even the `math` library, etc. But I also expose come backend functions in C/C++20. These will be published in the API section yet to come.

#### Examples

##### The EON engine macOS and Windows scripts.

To get a preCog project up and running is very simple. Create a text file in the root of your project called "cogfile.lua". This follows the same paradigm as make and cmake. In it write the following Lua code:

```
print'Hello, world!'
```

Save the file and type `preCog` at the command line (I used the awesome iterm2 terminal on macOS)! Instantly the program will display the version
and 'Hello, universe!' Not very useful, but it follows in a grand old tradition's footsteps. Now this example assumes that you ran the installer that placed preCog in some directory in your path. On macOS, this will be `/usr/local/bin` possibly naughty again, but it is convenient. If I ever get around to pushing preCog to brew It'll live in `/opt/` somewhere.

##### Your first *REAL* preCog script.

Now that we've done due diligence and shown that preCog simply wraps Lua 5.4.x at, we'll make our next script. This one will be so flippin' simple
you'll have a heart attack or die laughing, one of the two. I expect you have an existing codebase that you want to port from Cmake, Qmake, Make, Xcode, or Visual Studio over to it. Yay! My very first victim!! Mwuhahahaha. It's quite easy to accomplish your goal with a few lines of Lua.

Let's say we have an include directory, a `src/` directory, and a lib directory full of third-party code. At the end of the day what we want to do is generate a Visual Studio 2019 solution on Windows, a great Xcode 15 workspace on macOS targeting that OS, iOS and iPadOS, MSVC on Windows (including Community Edition, if you're an individual developer) and Ninja on Linux. That's a tall order. Let's see how preCog does:

###### macOS

```
-- Create a variable to hold a workspace named 'myProject' and assign it to the "wsp", historically pronounced: wosp or wasp (short for WOrkSPace).
(wsp = workspace:new 'myProject')
  : find_includes    'include'
  : include_path     'include'
  : find_sources     'src'
  : target 'console'
```

So this is the absolute simplest way to make a project you can load into the IDE or compiler for your platform. At the command line, all you need to do is invoke the tool with a command like this


```
$ preCog --help
```

That will give you a list of commands and options. If you just run `preCog` by itself you'll get the following output:

```
$ preCog pre-build system v1.0.0
```

But that's not very useful now, is it? Let's show you how to pre-build for Xcode, targeting iOS and macOS:

```
$ preCog xocde=macos,ios # Create a multi-target project that compiles macOS and iOS executables or libraries, etc.
$ preCog xcode=macos
$ preCog xcode
```

###### Something to sink your teeth into

Here is the full pre-cogfile from Cog's own cogfile.lua, targeting macOS. On disk, the file layout from your project directory is normally:

```
<project>/
<project>/include
<project>/src
```

This matches the example `myProject` Lua pre-build script.

But of course, this layout can be anything you want. Remember, you tell preCog where your files are and it will scan those directories for code. Just like we did earlier.

So without further ado, here's the preCog build script for macOS, the most complex of the lot, and a great example of what you can do with the tool.

```
--------------------------------------------------------------------------------
-- Create the 'wsp' object and name it; this is the final program name on disk.
--------------------------------------------------------------------------------

wsp = workspace:new'preCog'

--------------------------------------------------------------------------------
-- Build options.
--------------------------------------------------------------------------------

local BOOST_DIRECTORY = 'usr/share/boost/1.84.0'
local EON_DIRECTORY   = 'src/core/include'
local WINSDK_VERSION  = '10.0.19041.0'
local UNIVERSAL_BIN   = 'yes'
local APPLE_SILICON  = 'no'

--------------------------------------------------------------------------------
-- Create a new wsp under workspace to compile startup code.
--------------------------------------------------------------------------------

wsp:new'startup'
  : defines{'_DEBUG=1,DEBUG=1','NDEBUG=1'}
  : set_include_paths( BOOST_DIRECTORY )
  : prefix'src/core/include/eon/eon.h'
  : find_sources'src/bootseq/start'
  : universal'yes'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lz4.
--------------------------------------------------------------------------------

wsp:new'lz4'
  : defines('_DEBUG=1,DEBUG=1','NDEBUG=1')
  : set_include_paths'src/lz4/include'
  : find_sources'src/lz4/src'
  : universal'yes'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for lua.
--------------------------------------------------------------------------------

wsp:new'lua'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths'src/lua/5.4.4/lua'
  : find_sources'src/lua/5.4.4/src'
  : ignore'lua.c,luac.c'
  : universal'yes'
  : target'static'

--------------------------------------------------------------------------------
-- Setup the build settings for gfc.
--------------------------------------------------------------------------------

wsp:new'gfc'
  : defines( '_DEBUG=1,DEBUG=1', 'NDEBUG=1' )
  : set_include_paths( BOOST_DIRECTORY..[[,
    src/lz4/include,]]
  ..EON_DIRECTORY )
  : find_sources'src/core/src,src/core/include'
  : prefix'src/core/include/eon/eon.h'
  : skip_unity'f32.cpp'
  : universal'yes'
  : target'static'

--------------------------------------------------------------------------------
-- Create a new wsp under workspace to compile startup code.
--
-- The PLATFORM variable is one of android, ios, linux, osx, web and win. If you
-- name your platform specific directories like so then one line pulls in the
-- code for a specific platform.
--------------------------------------------------------------------------------

wsp:new'pal'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : set_include_paths(
    BOOST_DIRECTORY..','
  ..EON_DIRECTORY )
  : find_sources'src/pal/src/osx,src/pal/include'
  : prefix'src/core/include/eon/eon.h'
  : universal'yes'
  : target'static'

--------------------------------------------------------------------------------
-- Generate cog executable wsp.
--------------------------------------------------------------------------------

wsp:new'precog'
  : defines( '_DEBUG=1, DEBUG=1','NDEBUG=1' )
  : identifier'com.creepydollgames.cog'
  : organization'Brian Hapgood'
  : team'HE96RQ5ZY9'
  : set_include_paths( BOOST_DIRECTORY..[[,
    src/applications/include,
    src/lua/5.4.0,]]
  ..EON_DIRECTORY )
  : find_sources'src/applications/cog/src,src/applications/cog/include'
  -- Specify frameworks with no decoration and static libraries from other cog
  -- projects with full filename (pathing is allowed too).
  : link_with[[
      CoreFoundation,
      Foundation,
      libstartup.a,
      libgfc.a,
      liblua.a,
      libpal.a,
      liblz4.a]]
  : prefix'src/core/include/eon/eon.h'
  : universal'true'
  : target'console'
```

###### Awesome .vimrc for your convenience

I use Vim a LOT! My `.vimrc` is found below, with some magical folds I developed a long time ago, in a galaxy far, far, away....

```
"-------------------------------------------------------------------------------
"                    Sometimes Your Knight In Shining Armour
"                         Is Just A Retard In Tinfoil.
"-------------------------------------------------------------------------------
"                   The best method for accelerating a computer
"                      is the one that boosts it by 9.8 m/s2.
"-------------------------------------------------------------------------------

let g:SgRightMargin=49
let g:SgColumns=110

filetype on

if has("multi_byte")
  if &termencoding == ""
    let &termencoding = &encoding
  endif
  se encoding=utf-8
  setg fileencoding=utf-8
  "setg bomb
  se fileencodings=ucs-bom,utf-8,latin1
endif

"-------------------------------------------------+-----------------------------
"                                                 :
" OnVimEnter and its two helper functions         :
"                                                 :
"-------------------------------------------------+-----------------------------
"CountVisibleLines:{                              |

  fu! g:CountVisibleLines()
    fu! CountLinesInFolds()
      let i=match(getline('.'),':'.'{')
      if i>=0
        let g:SgFoldTable[g:SgFoldIndex]=line('.')
        let g:SgFoldIndex+=1
      el
        let i=match(getline('.'),'}'.':')
        if i>=0
        el
          if g:SgFoldIndex>0
            let g:SgFoldIndex-=1
            let g:SgFoldCount+=line('.')-g:SgFoldTable[g:SgFoldIndex]
          en
        en
      en
    endf
    let g:SgFoldTable=[]
    let g:SgFoldIndex=0
    let g:SgFoldCount=0
    exe ':g/./cal g:CountLinesInFolds()<cr>'
    let l:SgFoldCount=g:SgFoldCount
    unl g:SgFoldTable
    unl g:SgFoldIndex
    retu l:SgFoldCount
  endf

"}:                                               |
"SgSetRightMargin:{                               |

  fu! g:SgSetRightMargin(mid)
    let g:SgRightMargin=a:mid
    exe 'normal' 'maggVGzC`azz'
  endf

"}:                                               |
"OnVimEnter:{                                     |

  fu! g:OnVimEnter()
    let g:OnVimEntered = 1
    "Activate rainbow parenthesis
    "hi OverLength guifg=#ffffff guibg=#000000
    se nocp et nowrap nu ar cc=81
    hi StatusLine guibg=#304060
    "mat OverLength /\%81v.\+/
    if has('gui')
      se co=92 lines=82
    en
    se sw=2 ts=2
    if has('gui') && (has('win32')||has('win64'))
      sil! se gfn=Cousine:h10
      winpos 565 0
    elseif has('gui')
      sil! se gfn='JetBrains\ Mono:h14'
      if has('syntax')
        sil! colo mustang|syntax on
      en
    el
      if has('syntax')
        sil! colo newspaper|syntax on
      en
    en
  endf
  au!
  au VimEnter * :cal g:OnVimEnter()

  "Activation based on file type
  augro rainbow_cpp
    au!
    au FileType cxx,clojure,scheme RainbowParentheses
    au FileType cpp,clojure,scheme RainbowParentheses
    au FileType hpp,clojure,scheme RainbowParentheses
    au FileType  cc,clojure,scheme RainbowParentheses
    au FileType  cs,clojure,scheme RainbowParentheses
    au FileType  hh,clojure,scheme RainbowParentheses
    au FileType   h,clojure,scheme RainbowParentheses
  augroup end

"}:                                               |
"-------------------------------------------------+-----------------------------
"                                                 :
" Folding callback                                :
"                                                 :
"-------------------------------------------------+-----------------------------
"Fold text callback:{

  if has("folding")
    exe "se fmr=:"."{,}".": fdm=marker fdo=undo fdt=CustomFoldText fdc=5"
    se fdt=CustomFoldText() fcl=all
    fu! CustomFoldText()
      "-------------------------------------------+-----------------------------
      "Status column:{
        if v:version < 701
          retu foldtext()
        en
        let &fcs = substitute(&fcs,',\?fold:.','','gi')
        let nuw = (v:version < 701 ? 8 : &nuw)
"       hi folded guibg=#204060
        let statText = ''
        let numLines = (v:foldend-v:foldstart)-1
        if  numLines > 1
          let statText .= printf(
          \ "%d lines",
          \ numLines,
          \ )
        elsei numLines == 1
          let statText.= printf(
          \ "1 line ",
          \ v:foldstart,
          \ v:foldend
          \ )
        el
          let statText.= 'empty '
        en
        let percent = eval(numLines.'.0')/line('$')*100.0
        if  percent < 10.0
          let s=printf("%.2f",percent)
          let statText .= '  '.matchstr(s,'[0-9]\+\.[0-9][0-9]')
          let statText .= '%'
        elsei percent < 100.0
          let statText .= ' '.printf("%.2f%%",percent)
        el
          let statText .=printf("%.2f%%",min(percent,100.0))
        en
        if stridx(tolower(getline(v:foldstart)),'microsoft')>=0
          let statText.= ' WTF' "yahboo!
        elsei numLines > 100
          let statText.= '  :('
        elsei numLines > 75
          let statText.= '  :<'
        elsei numLines > 50
          let statText.= '  :|'
        elsei numLines > 25
          let statText.= '  :)'
        el
          let statText.= '    '
        en
        let eol = (&cc > 0) ? &cc-1 : 80
        let text = strpart(getline(v:foldstart),0,eol-strlen(statText))
      "}:
      "Formatting:{
        let text = strpart(text,0,stridx(text,strpart(&fmr,0,stridx(&fmr,','))))
        if strlen(text) > g:SgRightMargin
          let text = strpart(text,0,g:SgRightMargin)
        en
        let text      = substitute(text,'\t',repeat(' ',&ts),'g')
        let firstWord = match(text,'[a-zA-Z<>\[\]]')
        let nonWspace = match(text,'[!-/:-@\[-`{-~]')
        if nonWspace
          let text = repeat(' ',nonWspace).'+ '.strpart(text,firstWord)
        el
          let text = strpart(text,firstWord)
        en
        let align = eol-strlen(text)
        let text  = printf('%s%*s',text,align,statText)
        let text  = strpart(text,0,g:SgRightMargin).' |'.strpart(text,g:SgRightMargin+3)
      "}:
      "-------------------------------------------+-----------------------------
      retu text
    endf
  en

"}:
"-------------------------------------------------+-----------------------------
"                                                 :
" Keyboard shortcuts on function keys             :
"                                                 :
"-------------------------------------------------+-----------------------------
"Spacebar:{                                       |

  nn <c-space> zCVzC
  nn <s-space> @q
  nn <a-space> :se nofen!<cr>
  nn <space> za

"}:                                               |
"Indents:{                                        |

  se si "indentation

//}:                                              |
"-------------------------------------------------+-----------------------------
"                                                 :
" Final status line settings                      :
"                                                 :
"-------------------------------------------------+-----------------------------
"Status line stuff:{                              |

se stl=[%{strlen(&fenc)?&fenc:'none'}, "file encoding
se stl+=%{&ff}]     "file format
se stl+=%h          "help file flag
se stl+=%m          "modified flag
se stl+=%r          "read only flag
se stl+=%y          "filetype
se stl+=%*\ \ \ \   "spacing
se stl+=%t          "tail of the filename
se stl+=%=          "left/right separator
se stl+=%#warningmsg#
se stl+=%*\
se stl+=%=      "left/right separator
se stl+=%c,     "cursor column
se stl+=%l/%L   "cursor line/total lines
se stl+=\ %P    "percent through file
se fen

"}:                                               |
"-------------------------------------------------+-----------------------------
" vim:sw=2 ts=2
```

###### Forthcoming Features

In the near future, probably v1.0.2 or something like that, I will add more IDEs and build systems to preCog's list of supported IDEs, build engines, and compilers. I'll even add support for my old friend Cmake! In addition to those I want to make preCog a full build system too, but that might be overkill and outside the scope of this tool.

I also want to allow pre-building of any project type on any platform. This is slated for the magical and mystical v1.0.0! I think that will be pretty useful for people who don't own Macs or PCs or anything in between.

Support for C# projects so we can construct useful things for Unity, etc.

#### The End

Then you'll have a lovely window. Hit Command+B and your program is produced!
	
-Brian Hapgood
