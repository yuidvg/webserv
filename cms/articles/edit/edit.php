<?php
// 簡単なセキュリティチェック
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // CSRFトークンのチェックなど、セキュリティ対策を行う
    // ここでは省略

    // POSTデータを取得
    $articleTitle = $_POST['title'] ?? '無題';
    $articleContent = $_POST['content'] ?? '内容なし';

    // データベースに保存する処理をここで行う（省略）

    // 保存後、記事の一覧ページへリダイレクト
    header('Location: /article/');
    exit;
}

// GETリクエストの場合、編集フォームを表示
?>

<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <title>記事の編集</title>
</head>
<body>
    <h1>記事の編集</h1>
    <form action="/article/edit" method="post">
        <div>
            <label for="title">タイトル:</label>
            <input type="text" id="title" name="title" required>
        </div>
        <div>
            <label for="content">内容:</label>
            <textarea id="content" name="content" rows="10" required></textarea>
        </div>
        <div>
            <button type="submit">保存</button>
        </div>
    </form>
</body>
</html>
