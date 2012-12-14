

= とある世界で一番高速なBrainfuckインタプリタ

//raw[|html|<div style="text-align: right">hogelog</div>]

== まえがき

世の中にはたくさんのプログラミング言語が存在します。
Dennis RitchieはUnixを記述するためにC言語を作り
@<fn>{hoge-fn-1}，
Tecgrafチームはブラジルの貿易障壁の影響下で
Luaを
@<fn>{hoge-fn-2}，
そしてUrban M@<raw>{|html|&uuml;}llerは
チューリング完全な最小限の言語として
Brainfuckを作成しました
@<fn>{hoge-fn-3}
。
//footnote[hoge-fn-1][The Development of the C Language* http://cm.bell-labs.com/cm/cs/who/dmr/chist.html]
//footnote[hoge-fn-2][The Evolution of Luahttp://www.lua.org/doc/hopl.pdf]
//footnote[hoge-fn-3][http://www.muppetlabs.com/~breadbox/bf/]

世の中にはたくさんのプログラミング言語処理系が存在します。
例えばRuby言語には
まつもとゆきひろによるオリジナル実装を元にした
MRI@<fn>{hoge-fn-4}，
JVM上で動作する
JRuby@<fn>{hoge-fn-5}，
JITコンパイラや世代別GCなどが組み込まれた
Rubinius@<fn>{hoge-fn-6}，
組み込みスクリプトエンジン向けとして作られた
mruby@<fn>{hoge-fn-7}
など様々な処理系があります。
例えば大規模分散処理のためHadoopフレームワークを利用したければ
JRubyを使うこともあるでしょう。
エディタアプリケーションにスクリプト機能を組み込むためにはmrubyを
使うかもしれません。
言語処理系にとって重要な機能は
使う局面によって変わってきますが，
処理系が高速であればだいたいの人は嬉しいでしょう？
//footnote[hoge-fn-4][http://www.ruby-lang.org/ja/]
//footnote[hoge-fn-5][http://jruby.org/]
//footnote[hoge-fn-6][http://rubini.us/]
//footnote[hoge-fn-7][https://github.com/mruby/mruby/]

Brainfuckという言語は
@<tt>{+ - > < , . [ ]}
という8個の命令しか存在しない，
処理系の実装が容易な言語です
@<fn>{hoge-fn-8}。
以下ではBrainfuckインタプリタの高速化を通じて
@<b>{インタプリタの高速化}
技術について親しんでいきましょう。
とりあえずはこの記事のタイトル通りに
@<b>{世界で一番高速なBrainfuck処理系}
を作ってみましょう。
//footnote[hoge-fn-8][http://www.kmonos.net/alang/etc/brainfuck.phpとかわかりやすい解説だと思います]

なお以下で示すサンプルコードは
筆者のgithubリポジトリ
@<fn>{hoge-fn-9}
から適宜参照できます。
//footnote[hoge-fn-9][https://github.com/hogelog/sample-code/tree/master/dbtimes-vol01]

== 普通の処理系

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf.cpp

さて，まずはなんの高速化も施されていない
Brainfuck処理系を実装します。
（@<list>{bf.cpp}）

たったこれだけの記述で一応ちゃんとした
言語処理系になるんだから楽で良いですね。

//list[bf.cpp][普通のBrainfuckインタプリタ]{
#include <stdio.h>
#include <string>
int main() {
  std::string code;
  for (int ch;(ch=getchar())!=EOF;)
    code.push_back(ch);
  static int membuf[30000];
  int *mem = membuf;
  int depth = 0;
  for (size_t pc=0;pc<code.size();++pc) {
    switch(code[pc]) {
      case '+': ++*mem; break;
      case '-': -*mem; break;
      case '>': ++mem; break;
      case '<': -mem; break;
      case ',': *mem = getchar(); break;
      case '.': putchar(*mem); break;
      case '[':
        if (*mem == 0) {
          depth = 1;
          while (depth != 0) {
            switch(code[++pc]) {
              case '[': ++depth; break;
              case ']': -depth; break;
            }}}
        break;
      case ']':
        depth = -1;
        while (depth != 0) {
          switch(code[-pc]) {
            case '[': ++depth; break;
            case ']': -depth; break;
          }}
        -pc;
        break;
    }}
  return 0;
}
//}

== インタプリタ高速化

上述したインタプリタは簡潔ですしちゃんと動くので良いのですが
あまり高速ではないので，
いくつかの高速化を施して世界最速インタプリタにしてみましょう。

=== VM化

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf-vm.cpp

プログラムの文字列を逐次解釈するインタプリタを
高速化する手法として，
プログラムをVM@<fn>{hoge-fn-10}
命令列へ変換する部分と
VM命令列のインタプリタ部分に分ける方法がよく知られています。
RubyインタプリタのMRIは
バージョン1.9から
独自VMを内蔵するようになり，
大幅な高速化に成功しました。
ここではBrainfuckインタプリタの
VM化を試してみましょう。
//footnote[hoge-fn-10][Virtual Machineの略]

@<list>{bf.cpp}
のプログラムは
@<tt>{[ ]}命令を処理する部分に
while文があるのが気になります。
@<tt>{[}
と対応する
@<tt>{]}
は一度調べればわかるのに，
毎回1文字ずつ探すのは効率がよくありません。
@<tt>{[ ]}命令
を対応するVM命令の位置を保持するVM命令へと
変換することで，
@<tt>{[ ]}命令
の処理部分から
while文が消えます。
（@<list>{bf-vm.cpp:loop}）

またプログラムを実行するforループで毎回
@<tt>{pc<code.size()}
とチェックするのも無駄です。
処理するVM命令列の最後に
「処理を終了する」命令を一つ追加することで，
終了判定もswitch文の中に混ぜることができました。
（@<list>{bf-vm.cpp:finish}）

//list[bf-vm.cpp:structure][VM命令構造]{
struct Instruction {
    char op;
    int jmp;
};
//}

//list[bf-vm.cpp:loop][命令処理部分]{
case '[':
    if (*mem == 0) {
        pc = code.jmp;
    }   
    break;
case ']':
    pc = code.jmp;
    break;
//}

//list[bf-vm.cpp:finish][終了処理]{
    for (size_t pc=0;;++pc) {
        Instruction code = codes[pc];
        switch(code.op) {
            ...
            case '\0':
                return;
//}

==== direct threaded code

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf-vm2.cpp

VMの高速化手法の一つに
direct threaded code
と呼ばれる手法があります。
詳細は
Ruby 1.9
のVM化を主導した
笹田耕一さんによる記事
YARV Maniacs
@<fn>{hoge-fn-11}
に任せますが，
この手法により
命令処理部分に存在した大きな
switch文による分岐コストを
大幅に軽減できます。
//footnote[hoge-fn-11][http://jp.rubyist.net/magazine/?0008-YarvManiacs]

=== JITコンパイル

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf-jit.cpp

前の章ではVM化として
プログラムをVM命令に変換する部分と
VM命令を処理する部分に分けることによる
最適化について説明しました。
しかしどうせ変換するなら
プログラムを実際の機械語列に変換して，
その機械語列をそのままCPUに実行してもらえば
良いのです。
この手法はJITコンパイルと呼ばれています。
最近では各種ブラウザベンダが
「うちのブラウザのJavaScriptエンジンはJITコンパイラを内蔵してるから速いんだ！」
と主張しあってたりすることから
耳にする機会も多いでしょう。

Xbyak@<fn>{hoge-fn-12}，
asmjit@<fn>{hoge-fn-13}
などのJITアセンブラを
用いると，
各命令処理をアセンブラ相当の記述をするだけで
JITコンパイラができますし，
慣れてしまえば簡単ですね？
//footnote[hoge-fn-12][http://homepage1.nifty.com/herumi/soft/xbyak.html]
//footnote[hoge-fn-13][http://code.google.com/p/asmjit/]

=== 最適化

//list[mandelbrot.b][とある巨大なBrainfuckプログラム（抜粋）]{
+++++++++++++[->++>>>+++++>++>+<<<<<<]>>>>>++++++>ー>>>>>>>>>>+++++++++++++++[[
>>>>>>>>>]+[<<<<<<<<<]>>>>>>>>>-]+[>>>>>>>>[-]>]<<<<<<<<<[<<<<<<<<<]>>>>>>>>[-]+
<<<<<<<+++++[-[->>>>>>>>>+<<<<<<<<<]>>>>>>>>>]>>>>>>>+>>>>>>>>>>>>>>>>>>>>>>>>>>
>+<<<<<<<<<<<<<<<<<[<<<<<<<<<]>>>[-]+[>>>>>>[>>>>>>>[-]>>]<<<<<<<<<[<<<<<<<<<]>>
>>>>>[-]+<<<<<<++++[-[->>>>>>>>>+<<<<<<<<<]>>>>>>>>>]>>>>>>+<<<<<<+++++++[-[->>>
>>>>>>+<<<<<<<<<]>>>>>>>>>]>>>>>>+<<<<<<<<<<<<<<<<[<<<<<<<<<]>>>[[-]>>>>>>[>>>>>
>>[-<<<<<<+>>>>>>]<<<<<<[->>>>>>+<<+<<<+<]>>>>>>>>]<<<<<<<<<[<<<<<<<<<]>>>>>>>>>
[>>>>>>>>[-<<<<<<<+>>>>>>>]<<<<<<<[->>>>>>>+<<+<<<+<<]>>>>>>>>]<<<<<<<<<[<<<<<<<
<<]>>>>>>>[-<<<<<<<+>>>>>>>]<<<<<<<[->>>>>>>+<<+<<<<<]>>>>>>>>>+++++++++++++++[[
>>>>>>>>>]+>[-]>[-]>[-]>[-]>[-]>[-]>[-]>[-]>[-]<<<<<<<<<[<<<<<<<<<]>>>>>>>>>-]+[
//}

//list[mandelbrot.b:opt][とある巨大なBrainfuckプログラム（抜粋）を最適化]{
c[->cmc>c>+m]mc>cmc[[m]+[m]m-]+[mz>]m[m]mz+mc[-[-m+m]m]m+m+m[m]mz+[m[mzm]m[m]mz+
mc[-[-m+m]m]m+mc[-[-m+m]m]m+m[m]m[zm[m[-m+m]m[-m+m+m+<]m]m[m]m[m[-m+m]m[-m+m+m+m
]m]m[m]m[-m+m]m[-m+m+m]mc[[m]+>z>z>z>z>z>z>z>z>zm[m]m-]+[
//}

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf-vm-opt.cpp

https://github.com/hogelog/sample-code/blob/master/dbtimes-vol01/bf-jit-opt.cpp

ここまでは対象言語を問わない汎用的な高速化手法を紹介しました。
ここからはBrainfuck特有の性質に着目した最適化を施してみましょう。

@<list>{mandelbrot.b}
のプログラムをしばらく眺めてみると，いくつかの無駄が存在することに気づきます。
まずは
@<tt>{++++}，
@<tt>{ー-}，
@<tt>{>>>>}，
@<tt>{<<<<}
のような同じ命令の繰り返し。
@<i>{n}個の
@<tt>{+}命令
の繰り返しは
@<i>{n}加算する
@<tt>{c}命令，
@<i>{n}個の
@<tt>{-}命令
は
@<i>{-n}加算する
@<tt>{c}命令に置き換えられます。
同様に
@<tt>{>}
と
@<tt>{<}
も
@<tt>{m}
命令に
置き換えられます。

またよく出てくる命令の組み合わせに
@<tt>{[-]}
があります。
これは「メモリの値が0になるまで@<tt>{-}命令を繰り返す」という処理，
つまり「メモリの値を0にする」という
@<tt>{z}命令
に
置き換えてしまいましょう。

ここまでの最適化で
@<list>{mandelbrot.b}
のプログラムが
@<list>{mandelbrot.b:opt}
までに圧縮されます。
特に
@<tt>{z}命令
の導入により，
存在していたループ処理のいくつかが消えています。

このように，あとはひたすら
「こういう組み合わせよく出てくるからこういう特定の命令に置き換えよう」
という作業を続けるだけで，なんと世界で最速のBrainfuckインタプリタが
完成したのでした
@<fn>{hoge-fn-14}。
//footnote[hoge-fn-14][https://github.com/hogelog/fast-bf]

「世界最速の処理系だ」と主張しているのに，
まだ誰も「こっちの方が速い」
と言ってこないので，たぶん世界最速なんじゃないかな。
たぶん。きっと。おそらく。
