# statfs モジュール

以下のシステムコールの結果を返すモジュールです。

- statfs
- fstatfs
- getfsstat
- getmntinfo

システムコールの内容は<code>man</code>コマンド等で確認してください。

動作確認は

- macOS 15.6
- FreeBSD 14.3

で行っています。

## インストール

GNUmakefile を使用するので GNU make が必要です。

ユーザー向けにモジュールをインストール

```
% gmake install
```

上記の上書きインストールを実行

```
% gmake reinstall
```

## メソッド

モジュールのメソッドは以下の通り。

```
statfs(path: str) -> tuple
fstatfs(fd: int) ->  tuple
getfsstat(flags: int = MNT_NOWAIT) -> list
getmntinfo(flags: int = MNT_NOWAIT) -> list
```

メソッド<code>statfs,fstatfs</code>では<code>struct statfs</code>相当を<code>namedtuple</code>で返します。メンバ変数名は macOS,FreeBSD の両方を混ぜてますが、サポートしていない変数には<code>None</code>が設定されます。

メソッド<code>getfsstat,getmntinfo</code>では、「<code>struct statfs</code>相当の<code>namedtuple</code>」のリストを返します。
