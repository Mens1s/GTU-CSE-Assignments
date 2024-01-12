def minCoins(coins , target):
    if target <= 0:      # Base case: if target amount is 0  no coins needed and returns 0
        return 0

    minCount = 99999999  # TO Big number as infinity

    for coin in coins:
        if coin <= target:
            count = 1 + minCoins(coins, target - coin)  # Recursive call with reducing target amount by coin value
            if count < minCount:
                minCount = count  # Update minimum count if a new minimum is available
    return minCount

print(minCoins([1, 5, 10, 25], 63))  # 6
# it takes a lot of time to do this calculation because of the recursive calls and algorithm complexity