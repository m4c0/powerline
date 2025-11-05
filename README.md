# powerline

Super simple slide presentation tool.

I wrote it in an afternoon because I wanted a slideshow with a cool animation in the background.

You can control what's being presented by editing the "script.lsp" file. That file is a LISP-like S-expression supporting these functions:

 * `clear` - clears the screen
 * `pause` - pauses the presentation, waiting for you to press SPACE to continue
 * `title x y "text"` - draws a title in a given position
 * `text x y "text"` - draws a text in a given position

I don't have any concrete plans to maintain this tool, but feel free to engage with comments - I might take them in consideration if time allows.

Windows binary available on [itch.io][itch]. I am only distributing a Windows binary, but I might be able to provide MacOSX bundle if there is a demand.

Disclaimer: this software is provided as-is, without any liability. Do not use it for anything critical.

[itch]: https://m4c0.itch.io/powerline
