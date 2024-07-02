SELECT
    symbol
    , SUM(shares) AS shares
FROM transactions
WHERE user_id = ?
GROUP BY symbol

SELECT
    cash
FROM users
WHERE id = ?
