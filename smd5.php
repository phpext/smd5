<?php
    $secret=md5_encrypt("Wenz");
    echo "secret=".$secret.'<br/>';
    $text=md5_decrypt($secret);
    echo "text=".$text.'<br/>';
?>
