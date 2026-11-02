#q1
跟上次的py一样诶
long long ratio(long long hp, long long max_hp) { 
    if(max_hp <= 0) return -1;
    if(hp <= 0) return 0;
    if(max_hp > 0){          // ← 只在 >0 时返回
        if(hp >= max_hp) return 100;
        else return int(hp*100/max_hp);
    }
    // ← 这里没有 return：若 max_hp > 0 不成立，函数走到结尾
}
我这样写出现了ratio 函数可能从末尾“掉出去”没返回值的警告
所以我改成了直接把最后的判断删除，因为max_hp已经恒为真