

= データベースシステムの夜明け

//raw[|html|<div style="text-align: right">はやみず</div>]

データベースシステム無しでは，今日の社会は成り立たないと言って良いでしょう。社会が高度に情報化された現在，世界中で大量のデジタルデータが日々生み出され，飛び交い，消費され，そして蓄積されてゆきます。一方で，膨大なデジタルデータは，単に生み出され，蓄積されてゆくだけではゴミも同然です。必要なときに必要なデータが取り出せるよう，適切に管理してゆかなければなりません。そのための根幹たる存在がデータベースシステムなのです。

今日においては，「データベースシステム」と呼ばれているその殆どが「リレーショナルデータベースシステム」です。みなさんが馴染みのあるであろう MySQLや PostgreSQL，あるいは Oracle といったデータベースシステムは，すべてリレーショナルデータベースシステムですね。リレーショナルデータベースシステムの歴史を辿ると，その起源はある一篇の論文に遡ることができます。

その論文こそが，Edgar F.  Coddにより1970年に発表された @<raw>{|html|&ldquo;}A Relational Model of Data for Large Shared Data Banks@<raw>{|html|&rdquo;} です。この論文は，リレーショナルデータベースの最も重要な基礎となる@<b>{リレーショナルモデル}を提唱したもので，いわばデータベースシステム分野における金字塔です。古典力学をNewton が拓き，相対性理論を Einstein が拓いたとするならば，データベースの一大分野であるリレーショナルデータベースを拓いたのは Edgar F. Codd その人といって間違いないでしょう。

データモデルとは，どのような形式でデータを格納して，どのようにデータにアクセスするかということを取り決める枠組みです。データを蓄積・管理することを目的とするデータベースシステムにおいては，データモデルはその心臓部とも言える存在なのです。

そして，Codd によるリレーショナルモデル提唱から数年の後に，UNIXで動作する世界初のリレーショナルデータベースシステムの開発プロジェクトが立ち上がります。Michael Stonebraker率いる@<b>{INGRESプロジェクト}です。Coddにより確立されたリレーショナルデータベースシステムの基礎理論を，実際に動くソフトウェアとして実現し，そしてそれを世に広めたのがINGRESなのです。

本稿では，Coddによるリレーショナルモデルの提唱から，INGRESプロジェクトの黎明期の記録を辿り，現代の社会を支えるデータベースシステムがいかにして創り上げられたのか，その歴史を紐解いてみようと思います。

== リレーショナルモデルの登場

リレーショナルモデルが登場する以前，つまり1960年代までのデータベースシステムは殆どが階層型モデルやネットワーク型モデルというデータモデルに基づいて構築されていました。

//image[hierarchical-data-model][階層型モデル]{
//}

//image[network-data-model][ネットワーク型モデル]{
//}

階層型モデルというのは，木構造を用いてデータを組織化したデータモデルです。たとえば会社組織を階層型モデルで表すとしましょう。この会社では，会社の中に複数の部門があります。各部門の中では複数のプロジェクトが運営されています。そして各プロジェクトに従業員が属しています。そうすると，@<img>{hierarchical-data-model}のような階層構造でデータを組織化することができます。

ここで，例えば2つのプロジェクトHello projectとWorld projectに属する1人の社員Andrewがいたとしたらどうでしょう。階層型モデルにおいて，1つのデータの実体(社員やプロジェクト)が複数の親データに属することができません。つまり，1人の社員が複数のプロジェクトに属することを直接表現することができないのです。無理やりこれを表現しようとすると，プロジェクトごとにAndrewのデータを持つことになり，データの重複が生じてしまいます。また，部門Bと部門Cが共同でプロジェクトWorldを運営していることを表現しようと思うと，プロジェクトのデータに加えてその下にぶら下がっている社員のデータも併せてコピーしなければなりません。階層型モデルは非常にシンプルで性能が出しやすいというメリットもあるのですが，このようにデータ同士の関係を柔軟に記述することができないというデメリットもあるのです。

このような階層型モデルの欠点を克服したのがネットワーク型モデルです(@<img>{network-data-model})。ネットワーク型モデルでは，木構造ではなくグラフ構造によりデータの組織化を行います。グラフ構造であれば Andrew が複数プロジェクトに属することを自然に表現できるために，前述のようなデータ重複の問題は発生しません。

1960年代には，IBMによってIMS(Information Management System)という階層型モデルにもとづくデータベースシステムが開発されており，企業におけるデータ管理はIMSの独壇場ともいえる状況でした。1960年代の終盤には，ネットワーク型モデルの標準化団体としてCODASYLが立ち上がり，CODASYLデータモデルが標準規格として確立されます。

そんな中，IBM社内でこの動きに疑問を持つ一人の研究者がいました。この人こそ，後にリレーショナルモデルの提唱者となる Edgar F. Codd その人でした。

階層型モデルやネットワーク型モデルに基づくデータベースは，データのアクセスに一つ大きな問題を抱えていたのです。これらのデータベースにおいてデータの問い合わせを行う際には，データの構造がどのようになっているかを把握し，そしてどのような手順でデータを取得するかをプログラマ(データベースシステムのユーザ)が知っている必要がありました。例えば@<img>{network-data-model}のデータベースにおいて社員Andrewのデータを取得するためには，「会社のデータを読みとり，部門Bのデータを読み取り，プロジェクトHelloのデータを読み取り，社員Andrewのデータを読み取る」という手順を指定してあげなければなりません。このように，欲しいデータにアクセスするために，プログラマがデータの案内(navigation)をしなければならないという意味で，階層型モデルやネットワーク型モデルは@<b>{ナビゲーショナルモデル}，またそれにもとづくデータベースシステムは@<b>{ナビゲーショナルデータベースシステム}と呼ばれます。

もしもナビゲーショナルデータベースシステムにおいて，@<img>{network-data-model}の会社で組織改変があり，各部門の下には課が設置され，その下にプロジェクトが属するという構造にデータベースが修正されたとしたらどうなるでしょう。これまで社員のデータにアクセスするアプリケーションは会社→部門→プロジェクト→社員とたどっていましたが，会社→部門→課→プロジェクト→社員とたどるように修正しなければなりません。このようにナビゲーショナルデータベースシステムでは，その上位構造に変化があった時にはデータにアクセスする方法を再構成しなければなりませんでした。

当時のIBM社内では，データベースの仕様変更が生じるたびに，プログラマ達がデータアクセスのコード書き換えに追われていました。

この状況を打破するべく，Edgar F. Codd は1970年@<fn>{rise-fn-1} にリレーショナルモデルを提唱します。
//footnote[rise-fn-1][正確にはリレーショナルモデルの論文は1969年にIBMの社内技術報に掲載され，翌1970年に米国コンピュータ学会の論文誌に掲載されます。]

リレーショナルモデルでは，データの論理的な構造と物理的な構造を独立させることができるように考慮されています。そのため，データ構造を変更するたびにアプリケーションを書き直す必要がありません。またユーザは「どんなデータが欲しいか」を記述するたけで，具体的なアクセス方法はデータベース側が判断してデータを取得することができます。つまり，ナビゲーショナルデータベースシステムでは @<raw>{|html|&ldquo;}how@<raw>{|html|&rdquo;} を与えなければデータのアクセスが行えなかったのですが，リレーショナルデータベースは @<raw>{|html|&ldquo;}what@<raw>{|html|&rdquo;} を与えるだけでデータのアクセスが可能となります。

また，リレーショナルモデルは集合論にもとづく数学的基礎の上に成り立っており，後のデータベースシステム研究のための大きな理論的基盤となりました。これまでプログラマによる職人芸の上に成り立っていたデータベースシステムは，リレーショナルモデルの登場によって科学の領域へと押し上げられたのです。

== INGRESプロジェクト始動

1970年にリレーショナルモデルが提唱されてから，しばらく大きな動きはありませんでした。CoddがいたIBMにおいても，発表当初はあまり注目を集めることはなかったようです。

その一方で，1973年が終わろうとする頃，その論文が一人の若き研究者の目に留まります。その人こそ，INGRESプロジェクト，そして後のデータベース業界全体を率いる存在となる Michael Stonebraker です。Stonebraker は1971年にミシガン大学で博士号を取得し，UC Berkeleyで研究職についたばかりでした。テニュアトラック@<fn>{rise-fn-2} を走り始めたばかりのStonebraker にとって，リレーショナルモデルは恰好の研究対象だったのでしょう@<fn>{rise-fn-3}。リレーショナルモデルに出会ったStonebraker は，同僚の Eugene Wong と共にリレーショナルデータベースシステムの実装に乗り出します。こうしてINGRESプロジェクトは始まりました。
//footnote[rise-fn-2][テニュアとは大学における終身雇用資格のことです。若手研究者は任期の限られた研究職につき，テニュア獲得を目指して研究業績を積み上げてゆく，というのは米国における一般的な研究者のキャリアパスです。このテニュア獲得のための若手研究者が通る道をテニュアトラックと呼びます。テニュアトラックでいかに論文を@<raw>{|html|&ldquo;}量産@<raw>{|html|&rdquo;}できる研究ネタを選ぶかというのは，若手研究者にとっては人生に関わる重要な決断なのです。]
//footnote[rise-fn-3][ちなみに Stonebraker の博士論文は @<raw>{|html|&ldquo;}The Reduction of Large Scale Markov Models for Random Chains@<raw>{|html|&rdquo;} というタイトルで，もともとは数学寄りの研究をしていたようです。 ]

INGRESという名前は，@<b>{IN}teractive @<b>{G}raphic and @<b>{RE}trieval @<b>{S}ystem の頭文字をとったもので，本来は UC Berkeley の経済学の研究チームのために，地理情報をグラフィカルに表示するためのシステム研究として予算を獲得していたプロジェクトでした。そこは本音と建前で，まずはデータを効率的に取得するためのデータベースシステムが必要だ，ということで Stonebraker達はデータベースシステムの開発にのめりこんでゆきます。

こうして走り始めたINGRESプロジェクトですが，データベースシステムを開発するためのコンピュータはありませんし，そもそも Stonebraker や Wong は大きなプログラム開発の経験すらありませんでした。リレーショナルデータベースシステムの先駆けであるINGRESは，まさにゼロからのスタートだったのです。

何はなくともデータベースシステムの開発環境がなければ話になりません。INGRESプロジェクトが始まって Stonebraker が最初に取り組んだのは，コンピュータの調達と開発環境作りでした。当時といえば，データベースシステムを構築するためには高価なメインフレームを導入して，COBOLでプログラムを書くという時代でした。リレーショナルデータベースシステムのように，まだ世の中に存在すらしていないソフトウェアのために，研究職になりたての Stonebraker がメインフレームを手に入れることができるわけもありません。

そんな Stonebraker の目に留まったのが，当時新たな潮流として注目されつつあったミニコンピュータでした。ミニコンピュータとは，いわゆる現在のPCの先駆けとなる安価なコンピュータシステムのことです。1960年代に生まれ，そして1970年代にはそのビジネス活用の動きが高まり，小型ビジネスコンピュータ市場が生まれました。ミニコンピュータ市場の勃興に呼応するように，ベル研究所ではKen Thompson 達によってミニコンピュータ上で動作する汎用オペレーティングシステムUNIXの開発プロジェクトが始まりました。UNIXはもともとアセンブリで実装されていましたが，Dennis Ritchie 達によってUNIX開発のためにC言語が開発され，1973年にはC言語に移植されました。そう，まさにINGRESプロジェクトが始まったその年です。

ミニコンピュータ，そしてUNIXに出会った Stonebraker は，これを開発環境として使うことを決意します。開発マシンとして選ばれたのが，DEC(Digital Equipment Corporation)のPDP-11/70でした。ちなみに，UNIXが最初にベル研究所以外の場所でインストールされたのがこのINGRESプロジェクトであり，インストール時には Ken Thompson と Dennis Ritchie が5MBのディスクを抱えてやってきたそうです。

こうしてリレーショナルデータベースシステムの開発環境も整い，INGRESプロジェクトは動き始めました。

== INGRESの進化

INGRESプロジェクトでは，リレーショナルデータベースシステムの実装における重要な技術が多数生み出されてゆきました。当時のINGRESがどのように設計されていたのか，1976年に発表された論文@<raw>{|html|&ldquo;}The design and implementation of INGRES@<raw>{|html|&rdquo;}をもとに，その技術的な側面を概観してゆきます。

=== クエリ言語QUEL

リレーショナルデータベースでは，それ以前のナビゲーショナルデータベースとは異なり，どんなデータが欲しいか@<raw>{|html|&ldquo;}what@<raw>{|html|&rdquo;}を記述するだけで，具体的にどのようなデータ構造やアルゴリズムが実装されているかを一切知る必要なくデータを取得することができる，というのが大きなウリの一つです。その@<raw>{|html|&ldquo;}what@<raw>{|html|&rdquo;}を記述するための言語がクエリ言語です。皆さんのよく知っているSQLもクエリ言語の一つです。

INGRESプロジェクトでは，クエリ言語としてQUELが開発されます。リレーショナルモデルに併せて Codd はDSL/APLHAというクエリ言語を1971年に発表しているのですが，DSL/ALPHAは数学的な表記@<fn>{rise-fn-4}に基づいておりやや一般的にわかりやすいものではありませんでした。QUELはDSL/ALPHAをベースとして，数学的な表記のない@<raw>{|html|&ldquo;}やさしい@<raw>{|html|&rdquo;}クエリ言語として設計されました。
//footnote[rise-fn-4][DSL/ALPHAは関係演算(relational calculus)をベースとして，量化子@<raw>{|html|&exist;, &forall;}や，論理演算の記号@<raw>{|html|&not;,&and;,&or;}などがほぼそのまま用いられていました]

QUELがどんな言語なのか，簡単なサンプルコード@<fn>{rise-fn-5}をみてみましょう。
//footnote[rise-fn-5][1975年のINGRESの論文より引用]

//emlist{
RANGE OF C IS CITY
RETRIEVE INTO W(C.CNAME,
                      DENSITY = C.POPULATION / C.AREA)
         WHERE C.STATE = 'California'
            AND C.POPULATION > 50000
//}

このサンプルコードでは，カリフォルニア州にある人口5万人以上の市について，市の名前と人口密度の一覧を取得するという処理を行っています。一行目のRANGE文では，処理の対象としてCITYリレーションを選択して，CITYリレーションのタプルを表す変数名Cを宣言しています。二行目のRETRIEVE文は，SQLでいうところのSELECT文に相当します。処理の結果は，一時テーブルとして作成されるWに保存されます。細かい表記の違いを別にすると，現在使われているSQLとそれほど大きな違いがないことがわかると思います。

=== クエリ処理

1975年に発表された時点で，INGRESは複数のリレーションの結合を含むクエリをサポートしています。

データベースシステムをある程度使い込んだことがある人ならば，「ネステッドループ結合(ジョイン)」や「ハッシュ結合(ジョイン)」という言葉を聞いたことがあると思います。データベースの性能チューニングをやったことがある人は，クエリの実行プランを眺めてハッシュジョインが走るようにパラメタを調整して…なんていう記憶もあるのではないでしょうか。

当時のINGRESにおいては，今で言うネステッドループ結合が実装されていました。INGRESにおけるネステッドループ結合は，実は現在のデータベースシステムにおけるネステッドループ結合とアルゴリズムとしてはほとんど同じです。逆に言うと，リレーション結合のコアな処理というのは未だに30年以上前の技術から大きく変化していないということでもあります。色々と複雑で高度なことをやっているように見えるデータベースシステムも，細かい技術要素ごとに分解して眺めてみると，実は古の技術がそのまま生き残っていることもあるのです@<fn>{rise-fn-6}。
//footnote[rise-fn-6][もちろんハッシュ結合やソートマージ結合など別の結合方式も後から生み出されていますし，並列制御なんかは結構複雑で高度なことをやっています]

では，INGRESの具体的なクエリ処理エンジンの構造を概観してみましょう。INGRESのクエリ処理エンジンは@<raw>{|html|&ldquo;}One Variable Query Processor (OVQP)@<raw>{|html|&rdquo;}と呼ばれています。日本語にすると「1変数クエリ処理エンジン」ということになります。前節のクエリの例で@<b>{RANGE}文でリレーションに対して変数を宣言していましたが，「1変数クエリ処理エンジン」の「変数」とはこのリレーションを表す変数のことを指します。つまり，「1変数クエリ処理エンジン」とは同時にひとつのリレーションのみを処理することができるクエリ処理エンジンということです。

それじゃあ複数のリレーションの結合ができないじゃないか，と思われるかもしれません。複数のリレーションの結合とは，つまり複数の変数が組み合わさったクエリになるからです。このようなクエリを処理するために，INGRESでは事前に1つの複雑なクエリを，複数の1変数クエリに分解して，OVQPに放り込んで処理してゆきます。例を追いながらその処理を追いかけてみましょう。

//emlist{
RANGE OF E, M IS EMPLOYEE
RANGE OF D IS DEPT
RETRIEVE (E.NAME)
   WHERE E.SALARY > M.SALARY AND
            E.MANAGER = M.NAME AND
            E.DEPT  = D.DEPT AND
            D.FLOOR# = 1 AND
            E.AGE > 40
//}

このクエリでは，EMPLOYEE(従業員)リレーションとDEPT(部門)リレーションが定義されているデータベースから，1階で働いていて(@<i>{D.FLOOR}# = 1)，年齢が40歳より上で(@<i>{E.AGE} @<raw>{|html|&gt;} 40)，マネージャーより給料が高い(@<i>{E.SALARY} @<raw>{|html|&gt;} @<i>{M.SALARY} @<b>{AND} @<i>{E.MANAGER} = @<i>{M.NAME} )従業員のデータを抜き出す，という問い合わせを行っています。

===== Step 1. 

クエリが1変数かどうかを判定→3変数なので分解する

===== Step 2. 

次の2つのクエリを実行

//emlist[(1)]{
RANGE OF D IS DEPT
RETRIEVE INTO T1(D.DEPT)
   WHERE D.FLOOR# = 1
//}

//emlist[(2)]{
RANGE OF E IS EMPLOYEE
RETRIEVE INTO T2(E.NAME, E.SALARY, E.MANAGER, E.DEPT)
   WHERE E.AGE > 40
//}

こうすると，一時リレーション@<i>{T1}，@<i>{T2}を使って最初のクエリは次のようになります。

//emlist{
RANGE OF D IS T1
RANGE OF E IS T2
RANGE OF M IS EMPLOYEE
RETRIEVE (E.NAME)
   WHERE E.SALARY > M.SALARY AND
            E.MANAGER = M.NAME AND
            E.DEPT  = D.DEPT
//}

===== Step 3.

データ数の少ない一時テーブル，ここでは@<i>{T1}を変数置換用のリレーションとして選択

===== Step 4.

//noindent
@<b>{4.1}

@<i>{T1}の各タプルについて，元のクエリ中の変数@<i>{D}をタプルの値で置き換える

//emlist{
RANGE OF E IS T2
RANGE OF M IS EMPLOYEE
RETRIEVE (E.NAME)
   WHERE E.SALARY > M.SALARY AND
            E.MANAGER = M.NAME AND
            E.DEPT  = [value].DEPT
//}

これで元は3変数だったクエリが2変数になりました。

//noindent
@<b>{4.2}

変数を値で置き換えたクエリが1変数でない場合には，Step 1. にもどって同じ手順を繰り返し，変数を減らしてゆく

//noindent
@<b>{4.3}

変数を値で置き換えて実行した各クエリの結果を集約

このようにして，INGRESでは複数リレーションの結合を含む複雑なクエリを処理していました。現在のデータベースシステムにおいても，本質的な処理の流れは同じように実装されています。

=== アクセスメソッドとデータ構造

アクセスメソッドとは，その名の通りデータベースに保存されているデータにアクセスするための方法です。例えば，とある会社の従業員のデータがEMPリレーションに1万人分保存されていたとします。この中からある従業員1人分のタプルを取り出すには，どうすればよいでしょうか。もっともナイーブには，EMPリレーションを頭から全部スキャンして，目的の従業員のタプルを探すという方法が考えられます。これも立派なアクセスメソッドの一つですが，1人分のデータを探すにはちょっと効率が悪すぎます。従業員番号でタプルを並べておいて，二分探索するというやり方も簡単に思いつく方法です。これもアクセスメソッドの一つといえます。

アクセスメソッドには様々な実装方法があり，それぞれが得意とするデータアクセス，苦手とするデータアクセスのパターンがあります。データベースシステムのように総合的なデータの管理を行うためのシステムでは，複数のアクセスメソッドを利用できる必要があります。

==== アクセスメソッドインターフェース

INGRESには，複数のアクセスメソッドを利用するための共通インターフェースが定められています。INGRESで標準的に提供されるアクセスメソッドはすべてこのインターフェースを介して提供されており，またこのインターフェースに従って実装することで独自のアクセスメソッドを追加することができるように設計されています。

INGRESで定められているアクセスメソッドインターフェースは次の通りです:

  1. @<b>{OPENR}(@<i>{descriptor}, @<i>{mode}, @<i>{relation_name})
  2. @<b>{GET}(@<i>{descriptor}, @<i>{tid}, @<i>{limit_tid}, @<i>{tuple}, @<i>{next_flag})
  3. @<b>{FIND}(@<i>{descriptor}, @<i>{key}, @<i>{tid}, @<i>{key_type})
  4. @<b>{PARAMD}(@<i>{descriptor}, @<i>{access_characteristics_structure})
  5. @<b>{PARAMI}(@<i>{index_descriptor}, @<i>{access_characteristics_structure})
  6. @<b>{INSERT}(@<i>{descriptor}, @<i>{tuple})
  7. @<b>{REPLACE}(@<i>{descriptor}, @<i>{tid}, @<i>{new_tuple})
  8. @<b>{DELETE}(@<i>{descriptor}, @<i>{tid})
  9. @<b>{CLOSER}(@<i>{descriptor})

@<b>{OPENR} と @<b>{CLOSER} はプログラマにとってはファイル操作等でお馴染みのインターフェースでしょう。@<b>{OPENR} で指定したリレーションを開き，リレーションのディスクリプタを取得します。以降，このリレーションに対するアクセスはこのディスクリプタを以って行われます。一連のアクセスが完了した後は，@<b>{CLOSER} によってリレーションを閉じます。

@<b>{GET} は，タプルID(tid)をキーとしてタプルのデータを取得するための関数です。@<i>{tid}でタプルIDの下限を，@<i>{limit_tid}でタプルIDの上限を指定することができ，@<b>{GET}が呼び出されるとこの条件にマッチする最初のタプルを見つけてきて，そのデータを @<i>{tuple} に書き込みます。また，@<i>{next_flag}にTRUEを指定して@<b>{GET}を呼び出した場合には，次にマッチするタプルのIDを@<i>{tid}に書きこむため，連続してタプルを読みだしてゆくことができます。

@<b>{FIND}は，@<i>{key}で指定した検索キーにマッチするタプルIDを取得するための関数です。@<i>{key_type}で完全一致検索，範囲検索などの検索モードを指定することができます。

@<b>{PARAMD}，@<b>{PARAMI}は@<b>{FIND}において利用可能な検索キーの種類や，検索モードを調べるための関数です。それぞれ，データのリレーションと，索引のリレーションに利用されます。

@<b>{INSERT}はタプルを挿入するための関数，@<b>{REPLACE}は既に存在するタプルを更新するための関数，@<b>{DELETE}はタプルを削除するための関数です。

==== データ構造

INGRESでは，@<b>{ヒープ}，@<b>{ハッシュ}，@<b>{ISAM}，@<b>{圧縮ハッシュ}，@<b>{圧縮ISAM}の5種類のアクセスメソッドが提供されています。

@<b>{ヒープ}は単純にタプルを並べてゆくだけの最も単純な構造で，タプルを探すときは毎回線形探索を行います。非常にタプル数の少ないリレーションや，クエリの実行結果を格納する一時的なリレーションに適しています。

//image[page-layout-ingres][INGRESのページ構造]{
//}

@<b>{ハッシュ}と@<b>{ISAM}は，検索キーを用いて少量のタプルに対するアクセスを高速に行うことを可能とする構造です。@<b>{ハッシュ}はハッシュテーブルを使って，@<b>{ISAM}は探索木を使って，検索キーとタプルIDの関連付けを行うことで高速なアクセスを実現しています。INGRESでは，タプルIDは (ページ番号,ページ内ライン番号) という値のペアによって表現されているので，タプルIDさえわかれば目的のデータの在り処がわかります。

ページの構造は@<img>{page-layout-ingres}のようになっています。ページの先頭にはヘッダ情報があり，またページの末尾にはラインテーブルと呼ばれるデータが格納されています。ラインテーブルの各要素は，タプルのデータがページ内のどの位置にあるかのオフセット情報を保持しています。タプルIDに含まれているページ内ライン番号は，このラインテーブル内のインデックス番号となっているので，タプルIDさえわかればタプルのデータがどこにあるのかがわかるようになっています。この構造には，タプルの物理的な位置が変化しても，ラインテーブルを更新するだけで，タプルIDは変化することがないという利点があります。

==== アクセスメソッドまとめ

//image[access-method][検索キーを用いたアクセスメソッドによるタプルデータの取得]{
//}

検索キーが与えられてから，タプルのデータが得られるまでの一連の流れを@<img>{access-method}にまとめました。まずは検索キーが与えられると，@<b>{FIND}を呼び出してタプルIDを取得します。@<b>{FIND}の中では，ハッシュテーブルや木構造を用いて効率的にタプルIDの検索が行われます。そして，得られたタプルIDを引数として@<b>{GET}を呼び出すことで，ページを読み出し，タプルのデータが取得されます。

この基本的なデータアクセスの枠組みは，INGRESの頃に確立され，現在でも殆ど変わること無く引き継がれています。

ただ1980年頃まで，INGRESには @<raw>{|html|B<sup>+</sup>}-tree を用いたアクセスメソッドは実装されていませんでした。ISAMと@<raw>{|html|B<sup>+</sup>}-treeはよく似ているのですが，根本的な違いとしてISAMは静的なアクセスメソッドであるのに対し，@<raw>{|html|B<sup>+</sup>}-treeは動的なアクセスメソッドであるということです。どちらも多分木構造を索引として用いていますが，ISAMではデータの挿入や削除のたびに木構造を変更することはなく，適当なタイミングでまとめて木構造を作り直します。そのため，頻繁にデータの挿入，削除が行われる場合にはどんどん構造劣化が進み，アクセス性能が落ちてしまいますが，参照が非常に多い場合には良い性能を発揮します。一方，@<raw>{|html|B<sup>+</sup>}-treeはデータの挿入，削除のたびに木構造を動的にバランスさせるために，ISAMで起きるような構造劣化は起きません。しかし，1回の処理あたりのオーバーヘッドはISAMよりも大きくなってしまいます。

INGRESの開発において，何度も@<raw>{|html|B<sup>+</sup>}-treeの導入が検討され，その度に採用が見送られてきたようです。しかしながら，1980年のINGRES開発を振り返る論文において，Stonebrakerは@<raw>{|html|B<sup>+</sup>}-treeの採用を見送ってきたのは @<raw>{|html|&ldquo;}major mistakes@<raw>{|html|&rdquo;}の一つであり，次にアクセスメソッドを再検討する際には@<raw>{|html|B<sup>+</sup>}-treeも実装するであろうと述べています。

また，INGRESのアクセスメソッドはUNIXのファイルシステムの上に構築されていたのですが，UNIXのファイルシステムによるオーバーヘッドが当初予測していたよりも深刻なものであり，独自のファイルシステムを実装するべきであった，とも同論文では述べられています。実際に，3年後に発表された論文においてStonebraker 達は独自に実装したファイルシステムを使った場合と性能比較を行っており，最大で70%程度性能が向上することを確認しています。このようにINGRESの開発過程で得られた知見は，UNIXの開発にフィードバックされ，その発展に大きく貢献しました。

=== 人材育成の場としてのINGRES

INGRESプロジェクトは，初期のリレーショナルデータベース実装における技術を確立するという非常に重要な功績を成し遂げました。しかし，INGRESプロジェクトの功績はそれだけにとどまりません。後のデータベース業界を牽引する人材を輩出したという点においても，その価値が評価されています。

INGRESプロジェクトの最初のチーフプログラマを務めた Gerald Held はTandem Computers において NonStop SQL の開発に携わります。また別の時期にチーフプログラマを務めた Robert Epstein は Sybase 社を設立してSybase の開発を行います。また Peter Krepps はINGRES商用化に際して立ち上げられた Relational Technology 社でプログラマとして活躍しました。

== System R

INGRESプロジェクトからすこし遅れて，IBMでもリレーショナルデータベースシステム System R の研究開発が始まります。リレーショナルモデルの父であるCodd がIBMで働いていたにも関わらず，歴史を振り返るとIBMはリレーショナルデータベースシステム開発において常に後塵を拝しています。

当時のIBMではIMSが圧倒的な商業的成功を収めていました。そのため，IMSの開発のために多くの投資を行っているIBMとしては，よくわからない数学で記述されたリレーショナルモデルは注目に値しないものだと思われたのでしょう。そのため，リレーショナルモデルにもとづくデータベースシステムとしてSysmte Rの研究開発が始まっても，スタッフとしては10人程度しか人員が配置されなかったようです。

IMSがベースとする階層型モデルはナビゲーショナルなので，プログラマがデータアクセスの方法を記述しなければなりませんでした。しかし見方を変えると，データベースのことを熟知しているプログラマにとっては，最善の方法でデータアクセスを行うことができるということでもあります。一方，リレーショナルモデルではプログラマが明示的にデータアクセスの方法を制御できません。究極的なパフォーマンスの観点からすると，ナビゲーショナルモデルが優位なのは確かです。実際に，究極の性能が求められるミッションクリティカルなシステムでは，未だにナビゲーショナルデータベースシステムが用いられているようです@<fn>{rise-fn-7}。
//footnote[rise-fn-7][JALのシステムにもIMSが使われているとか]

その後，リレーショナル陣営とナビゲーショナル陣営は激しい論争を繰り広げてゆきますが，次第に世の中の流れはリレーショナルモデルに傾いてゆきました。歴史を振り返ると，今やアセンブラを直接書くことが殆どないように，コンピュータの性能が良くなるにつれて技術のトレンドが変化するというのはままあることです。データベースシステムにおいても，最初は性能が低くて受け入れられないと思われていたリレーショナルモデルが，その利便性が故に主流となってゆきました。

== エピローグ

1970年代，Coddによるリレーショナルモデルの提唱に始まり，INGRESやSystem R の研究によってリレーショナルデータベースシステムが実用に耐えうるものであることが証明されました。1981年には，Coddはその功績を認められ計算機科学分野において最も権威あるチューリング賞を受賞します。

そして1970年代の終盤から1980年代は，商用リレーショナルデータベースシステムが次々と登場し，熾烈な戦いを繰り広げてゆきます。

1977年，現在の Oracle の前身である Software Development Laboratories がLarry Ellison と Robert Miner によって設立され，1979年には世界初のSQLベースの商用リレーショナルデータベースシステム Oracle RDBMS をリリースします。

1979年，Jack Shemer，Philip Neches，Walter Muir，Jerold Modes，William Worth，Carroll ReedによってTeradataが設立され，1984年に世界初の並列データベースシステムDBC 1012@<fn>{rise-fn-8}をリリースします。
//footnote[rise-fn-8][ちなみに1012というのは@<raw>{|html|10<sup>12</sup>}=1TBにちなんでつけられた名前です]

1980年，Lawrence Rowe，Michael Stonebraker，Eugene Wong，Gary Morgenthaler によって INGRES 商用化のために Relational Technology (後のIngres Corp.)が設立され，翌1981年に商用INGRESをリリースします。一方，UC Berkeley においても INGRES プロジェクトは継続しており，こちらのほうはUniversity INGRES と呼ばれるようになります。University INGRES のソースコードは現在UC Berkeleyのウェブサイトからダウンロード可能です@<fn>{rise-fn-9}。
//footnote[rise-fn-9][http://s2k-ftp.cs.berkeley.edu/ingres/]

時を同じくして，Roger Sippl と Laura King によって 1980年に Relational Database Systems (後の Informix Software) が設立され，翌1981年にInformix がリリースされます。

IBMは1981年にSQL/DSを，さらに1983年にはDB2をリリースしてリレーショナルデータベース市場に乗り出します。エンタープライズ市場の覇者であるIBMのこの動きは，旧来のナビゲーショナルデータベースシステムに対する新興勢力リレーショナルデータベースシステムの勝利を決定的なものにします。

1984年，Mark Hoffman，Bob Epstein，Jane Doughty，Tom HagginによってSybaseが設立され，1986年にリレーショナルデータベースシステムSybaseをリリースします。後にSybaseはMicrosoftと提携してSybase SQL Serverを開発しました。その設計は現在のMicrosoft SQL Serverにも受け継がれています。

同1984年，ミニコンピュータ市場を牽引するDECがOpenVMSオペレーティングシステムにおける機能として DEC Rdb を発表します。

1986年，無停止コンピュータシステムを主な事業として手がける Tandem Computers が NonStop SQL をリリースします。

そして1990年代には更にプレイヤーが増えつつもその統廃合が進み，オブジェクト指向データベースシステムなど新興技術の登場，オープンソースのデータベースシステム隆盛の兆候など，データベースシステムは新たな時代へと突入してゆきます。

== 参考文献

  * E. F. Codd, @<raw>{|html|&ldquo;}A Relational Model of Data for Large Shared DataBanks@<raw>{|html|&rdquo;}, @<i>{Communications of the ACM}, Volume 13, Number 6,1970
  * E. F. Codd, @<raw>{|html|&ldquo;}A data base sublanguage founded on the relationalcalculus@<raw>{|html|&rdquo;}, @<i>{Proceedings of the 1971 ACM SIGFIDET workshopon Data Description, Access and Control}, 1971
  * C. J. Date, E. F. Codd, @<raw>{|html|&ldquo;}The relational and network approaches: Comparison of theapplication programming interfaces@<raw>{|html|&rdquo;}, @<i>{Proceedings of the1974 ACM SIGFIDET workshop on Data Description, Access andControl}, 1975
  * G. Held, M. Stonebraker, E. Wong, @<raw>{|html|&ldquo;}INGRES-A relational database management system@<raw>{|html|&rdquo;}, @<i>{Proceedings of AFIPS 1975 NCC},Volume 44, 1975
  * M. Stonebraker, G. Held, E. Wong, P. Kreps, @<raw>{|html|&ldquo;}The design andimplementation of INGRES@<raw>{|html|&rdquo;}, @<i>{ACM Transactions on DatabaseSystems}, Volume 1, Issue 3, 1976
  * G. Held, M. Stonebraker, @<raw>{|html|&ldquo;}B-trees re-examined@<raw>{|html|&rdquo;}, @<i>{Communications of theACM}, Volume 21, Issue 2, 1978
  * M. Stonebraker, @<raw>{|html|&ldquo;}Retrospection on a database system@<raw>{|html|&rdquo;}, @<i>{ACMTransactions on Database Systems}, Volume 5, Issue 2, 1980
  * D. Chamberlin, M. Astrahan, M. Blasgen, J. Gray, W. F. King,B. Lindsay, R. Lorie, J. Mehl, T. Price, F. Putzolu, P. Selinger,M. Schkolnick, D. Slutz, I. Traiger, B. Wade, R. Yost, @<raw>{|html|&ldquo;}Ahistory and evaluation of System R@<raw>{|html|&rdquo;}, @<i>{Communications of theACM}, Volume 24, Issue 10, 1981
  * M. Stonebraker, J. Woodfill, J. Ranstrom, M. Murphy, M. Meyer,E. Allman, @<raw>{|html|&ldquo;}Performance enhancements to a relational databasesystem@<raw>{|html|&rdquo;}, @<i>{ACM Transactions on Database Systems}, Volume 8,Issue 2, 1983
  * Relational Technology Inc., @<raw>{|html|&ldquo;}INGRES The Distributed SQLRelational Database System Press Kit@<raw>{|html|&rdquo;}, 1987
  * National Research Council, @<raw>{|html|&ldquo;}Funding a Revolution: GovernmentSupport for Computing Research@<raw>{|html|&rdquo;}, Natl Academy Pr, 1999
  * M. Stonebraker, J. M. Hellerstein, @<raw>{|html|&ldquo;}What Goes Around ComesAround@<raw>{|html|&rdquo;}, @<i>{Readings in Database Systems, Fourth Edition},Morgan Kaufmann, 2005
  * A. Ances, @<raw>{|html|&ldquo;}RDBMS Workshop: Ingres and Sybase@<raw>{|html|&rdquo;}, ComputerHistory Museum, 2007

and so many online resources
