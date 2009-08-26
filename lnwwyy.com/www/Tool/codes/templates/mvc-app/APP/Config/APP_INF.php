<?php

// 定义应用程序设置
// 更多设置参考 FleaPHP 文档
return array(
    /**
     * 是否将 url 参数中包含的控制器名字和动作名字强制转为小写
     * 可以避免在 url 中输入大写字符导致找不到控制器
     */
    'urlLowerChar'              => true,

    /**
     * FleaPHP 内部及 cache 系列函数使用的缓存目录
     */
    'internalCacheDir'          => APP_DIR . DS . '_Cache',
);
