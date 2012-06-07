Database Times Vol.01 について
==============================

C82参加申請サークル情報
-----------------------

Circle.ms 申し込み履歴より抜粋

  * サークル名：Hotchpotch Society (ホッチポッチソサイエティ)
  * 代表者名：はやみず
  * 発行誌名：Database Times
  * ジャンルコード：240同人ソフト
  * ジャンル補足：コンピュータ(情報・評論)
  * 配置：2012/8/11(土)  西地区“し”ブロック－29b
  * 頒布物概要：下記

データベースシステム(DBS)の今と昔を切り取り解説した冊子の頒布を予定して
います。データ有るところにDBS在り。データの形がかわる度に、DBSも変わり
続けています。DBSの最前線で何が起きているのか、最近話題のKey-Valueスト
アやBig Dataとは何なのか、などなどの記事を掲載予定です。


執筆者各位へ
============

執筆記事のテーマ
----------------

コミケにはデータベースのネタで申請してありますが、はやみずがデータベー
スネタの記事を2〜3本書く予定なのでそれを大義名分とします。よって、各自
ジャンルコードから大きく外れない範囲で好きなテーマで書いてください。具
体的には、ITに関する技術的解説やチュートリアル、近年の技術動向に関する
評論など。

締切について
------------

  * Soft deadline: 2012/07/19(Thu) 23:59 (JST)
  * Hard deadline: 2012/07/26(Thu) 23:59 (JST)

執筆者と執筆ネタリスト(各自適当に更新してください)
--------------------------------------------------

  * hayamiz
    * まえがき
    * PostgreSQL Conf 2011 レポ
    * PostgreSQLの内部構造解説
  * suu_g
  * yuyarin
  * hogelog
    * Adobe Alchemyが云々
    * もしくはBrainf*ckインタプリタとかの話
    * もしくは黒歴史


原稿執筆の進め方
================

pLaTeXを利用して組版を行うので、原則として TeX ファイルで原稿を書くこと
を推奨します。別のDTPソフトを用いたい場合には応相談。その場合には、ペー
ジの割振り決定後にノンブル(ページ番号)を各自で打つ、フォントを埋めこむ
など、入稿に必要な作業を自分でやってもらうことになると思います。

推奨執筆環境
------------

  * TeX Live 2011
    * install-tl コマンドで /usr/local/texlive に閉じた環境を構築できる
    * 参考: http://blog.livedoor.jp/vine_user/archives/51909130.html
  * 文字コードはUTF-8
  * OMake

原稿のコンパイル
----------------

pLaTeX、および必要なパッケージがインストールされた環境で下記のコマンド
を実行すると原稿がコンパイルされ、book.dvi および book.pdf が生成されま
す。

    $ omake

ただし、上記コマンドで生成される book.pdf はフォントが埋め込まれていな
いため、日本語フォントの表示が環境によって異なります。最終的に印刷会社
へ入稿するデータは、和文フォントに小塚フォントを埋め込んだものにする予
定です。小塚フォントが手元にある場合には、下記のコマンドで最終的なPDFファ
イルを生成することができます。

    $ omake book-ready.pdf

小塚フォントを持っていない人もいると思うので、ビルドサーバを立てました。

  * Jenkins: http://jenkins.hayamiz.com/job/dbtimes-vol01/
    * Github に push するとビルドが始まる
  * 生成されるPDF置場: http://hayamiz.com/~haya/files/dbtimes-vol01/
    * 一応Basic認証をかけてある
      * username: hotchpotch
      * password: station

小塚フォントを持っていない人もいると思うので、近日中にビルドサーバを立
てて、このgitレポジトリにコミットされるたびに小塚フォントの埋め込まれた
PDFを生成できるようにする予定です。

ファイル構成
------------

    dbtimes-vol01/
     +- OMakefile
     +- OMakeroot
     +- book.tex
     +- hayamiz/
         +- images/
             +- ...
         +- OMakefile
         +- preface.tex
         +- pgcon-report.tex
           ...
     +- <your name>/
         +- article01.tex
         +- article02.tex
           ...

各自の名前でディレクトリを作成して、自分の書いた記事のファイルを
book.tex で input するよう、全体のファイルを構成します。記事のファイル
を作成したら、book.tex に input 文を各自追加してください。ただし、
book.tex から input で読み込まれるファイルの文字コードはUTF-8にする必要
があります。執筆環境にUTF-8対応のpLaTeXが無い場合は、nkf 等でUTF-8に変
換したファイルを生成し、それを input するように注意してください。

上記のようなファイル構成をとることで、自分のディレクトリの中に自分専用
の pLaTeX コンパイル環境を構築することが可能です。ルートディレクトリに
ある OMakefile や book.tex は、主にはやみずの環境に最適化されているので、
各自好きにやってください。
