smd5
====

MD5解密 - 学习例子  
通过curl请求http://tool.zzblo.com/api/md5/encrypt，http://tool.zzblo.com/api/md5/decrypt 达到解密的目的.

安装：
====
```
./phpize
./configure --with-php-config=/path/php-config
make && make install
```

##默认配置：
smd5.decrypt_url=http://tool.zzblo.com/api/md5/decrypt  
smd5.encrypt_url=http://tool.zzblo.com/api/md5/encrypt  
你也可以在php.ini中更改该API ，API格式可参考这篇文章:http://silenceper.com/archives/1145.html

使用方法：
====
提供了md5_encrypt、md5_decrypt两个MD5加密以及解密方法
```php
<?php
    $secret=md5_encrypt("Wenz");
    echo "secret=".$secret.'<br/>';
    $text=md5_decrypt($secret);
    echo "text=".$text.'<br/>';
?>
```
