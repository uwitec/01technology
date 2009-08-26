<?php
/////////////////////////////////////////////////////////////////////////////
// FleaPHP Framework
//
// Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
//
// 许可协议，请查看源代码中附带的 LICENSE.txt 文件，
// 或者访问 http://www.fleaphp.org/ 获得详细信息。
/////////////////////////////////////////////////////////////////////////////

/**
 * 定义 FLEA_Helper_ImgCode 类
 *
 * @copyright Copyright (c) 2005 - 2008 QeeYuan China Inc. (http://www.qeeyuan.com)
 * @author 起源科技 (www.qeeyuan.com)
 * @package Core
 * @version $Id: ImgCode.php 972 2007-10-09 20:56:54Z qeeyuan $
 */

/**
 * FLEA_Helper_ImgCode 类实现了一个简单的图像验证码生成器，并带有检查验证方法
 *
 * 当启用了 session 时，验证码会保存在 session 中。用法：
 *
 * 模版页面中，在需要显示验证码的地方使用
 * <code>
 * <img src="<?php echo $this->_url('imgcode'); ?>" />
 * </code>
 *
 * 接下来为显示验证码的控制器编写 imgcode 方法：
 * <code>
 * function actionImgcode() {
 *     $imgcode =& FLEA::getSingleton('FLEA_Helper_ImgCode');
 *     $imgcode->image();
 * }
 * </code>
 *
 * 最后，对于用户提交的表单做如下验证：
 * <code>
 * function actionSubmit() {
 *     $imgcode =& FLEA::getSingleton('FLEA_Helper_ImgCode');
 *     // 假定验证码在表单中的字段名是 imgcode
 *     if ($imgcode->check($_POST['imgcode'])) {
 *         // 验证通过
 *     }
 * }
 * </code>
 *
 * @package Core
 * @author 起源科技 (www.qeeyuan.com)
 * @version 1.0
 */
class FLEA_Helper_ImgCode
{
    /**
     * 生成的验证码
     *
     * @var string
     */
    var $_code;

    /**
     * 验证码过期时间
     *
     * @var string
     */
    var $_expired;

    /**
     * 验证码图片的类型（默认为 jpeg）
     *
     * @var string
     */
    var $imagetype = 'jpeg';

    /**
     * 指示是否在生成验证码图片时保留已有的验证码
     *
     * 保留已有的验证码可以让用户在各个不同的页面都看到一致的验证码。
     * 只有这个验证码使用后，已有的验证码才会失效。
     *
     * @var boolean
     */
    var $keepCode = false;

    /**
     * 构造函数
     */
    function FLEA_Helper_ImgCode()
    {
        @session_start();

        $this->_code = isset($_SESSION['IMGCODE']) ?
                $_SESSION['IMGCODE'] : '';
        $this->_expired = isset($_SESSION['IMGCODE_EXPIRED']) ?
                $_SESSION['IMGCODE_EXPIRED'] : 0;
    }

    /**
     * 检查图像验证码是否有效
     *
     * @param string $code
     *
     * @return boolean
     */
    function check($code)
    {
        $time = time();
        if ($time >= $this->_expired || strtoupper($code) != strtoupper($this->_code)) {
            return false;
        }
        return true;
    }

    /**
     * 检查图像验证码是否有效（区分大小写）
     *
     * @param string $code
     *
     * @return boolean
     */
    function checkCaseSensitive($code)
    {
        $time = time();
        if ($time >= $this->_expired || $code != $this->_code) {
            return false;
        }
        return true;
    }

    /**
     * 清除 session 中的 imgcode 相关信息
     */
    function clear()
    {
        unset($_SESSION['IMGCODE']);
        unset($_SESSION['IMGCODE_EXPIRED']);
    }

    /**
     * 利用 GD 库产生验证码图像
     *
     * 目前 $options 参数支持下列选项：
     * -  paddingLeft, paddingRight, paddingTop, paddingBottom
     * -  border, borderColor
     * -  font, color, bgcolor
     *
     * 如果 font 为 0-5，则使用 GD 库内置的字体。
     * 如果要指定字体文件，则 font 选项必须为字体文件的绝对路径，例如：
     * <code>
     * $options = array('font' => '/var/www/example/myfont.gdf');
     * image($type, $length, $lefttime, $options);
     * </code>
     *
     * @param int $type 验证码包含的字符类型，0 - 数字、1 - 字母、其他值 - 数字和字母
     * @param int $length 验证码长度
     * @param int $leftime 验证码有效时间（秒）
     * @param array $options 附加选项，可以指定字体、宽度和高度等参数
     */
    function image($type = 0, $length = 4, $lefttime = 900, $options = null)
    {
        if ($this->keepCode && $this->_code != '') {
            $code = $this->_code;
        } else {
            // 生成验证码
            switch ($type) {
            case 0:
                $seed = '0123456789';
                break;
            case 1:
                $seed = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
                break;
            default:
                $seed = '346789ABCDEFGHJKLMNPQRTUVWXYabcdefghjklmnpqrtuvwxy';
            }
            if ($length <= 0) { $length = 4; }
            $code = '';
            list($usec, $sec) = explode(" ", microtime());
            srand($sec + $usec * 100000);
            $len = strlen($seed) - 1;
            for ($i = 0; $i < $length; $i++) {
                $code .= substr($seed, rand(0, $len), 1);
            }
            $_SESSION['IMGCODE'] = $code;
        }
        $_SESSION['IMGCODE_EXPIRED'] = time() + $lefttime;

        // 设置选项
        $paddingLeft = isset($options['paddingLeft']) ?
                (int)$options['paddingLeft'] : 3;
        $paddingRight = isset($options['paddingRight']) ?
                (int)$options['paddingRight'] : 3;
        $paddingTop = isset($options['paddingTop']) ?
                (int)$options['paddingTop'] : 2;
        $paddingBottom = isset($options['paddingBottom']) ?
                (int)$options['paddingBottom'] : 2;
        $color = isset($options['color']) ? $options['color'] : '0xffffff';
        $bgcolor = isset($options['bgcolor']) ? $options['bgcolor'] : '0x666666';
        $border = isset($options['border']) ? (int)$options['border'] : 1;
        $bdColor = isset($options['borderColor']) ? $options['borderColor'] : '0x000000';

        // 确定要使用的字体
        if (!isset($options['font'])) {
            $font = 5;
        } else if (is_int($options['font'])) {
            $font = (int)$options['font'];
            if ($font < 0 || $font > 5) { $font = 5; }
        } else {
            $font = imageloadfont($options['font']);
        }

        // 确定字体宽度和高度
        $fontWidth = imagefontwidth($font);
        $fontHeight = imagefontheight($font);

        // 确定图像的宽度和高度
        $width = $fontWidth * strlen($code) + $paddingLeft + $paddingRight +
                $border * 2 + 1;
        $height = $fontHeight + $paddingTop + $paddingBottom + $border * 2 + 1;

        // 创建图像
        $img = imagecreate($width, $height);

        // 绘制边框
        if ($border) {
            list($r, $g, $b) = $this->_hex2rgb($bdColor);
            $borderColor = imagecolorallocate($img, $r, $g, $b);
            imagefilledrectangle($img, 0, 0, $width, $height, $borderColor);
        }

        // 绘制背景
        list($r, $g, $b) = $this->_hex2rgb($bgcolor);
        $backgroundColor = imagecolorallocate($img, $r, $g, $b);
        imagefilledrectangle($img, $border, $border,
                $width - $border - 1, $height - $border - 1, $backgroundColor);

        // 绘制文字
        list($r, $g, $b) = $this->_hex2rgb($color);
        $textColor = imagecolorallocate($img, $r, $g, $b);
        imagestring($img, $font, $paddingLeft + $border, $paddingTop + $border,
                $code, $textColor);

        // 输出图像
        switch (strtolower($this->imagetype)) {
        case 'png':
            header("Content-type: " . image_type_to_mime_type(IMAGETYPE_PNG));
            imagepng($img);
            break;
        case 'gif':
            header("Content-type: " . image_type_to_mime_type(IMAGETYPE_GIF));
            imagegif($img);
            break;
        case 'jpg':
        default:
            header("Content-type: " . image_type_to_mime_type(IMAGETYPE_JPEG));
            imagejpeg($img);
        }

        imagedestroy($img);
        unset($img);
    }

    /**
     * 将 16 进制颜色值转换为 rgb 值
     *
     * @param string $hex
     *
     * @return array
     */
    function _hex2rgb($color, $defualt = 'ffffff')
    {
        $color = strtolower($color);
        if (substr($color, 0, 2) == '0x') {
            $color = substr($color, 2);
        } elseif (substr($color, 0, 1) == '#') {
            $color = substr($color, 1);
        }
        $l = strlen($color);
        if ($l == 3) {
            $r = hexdec(substr($color, 0, 1));
            $g = hexdec(substr($color, 1, 1));
            $b = hexdec(substr($color, 2, 1));
            return array($r, $g, $b);
        } elseif ($l != 6) {
            $color = $defualt;
        }

        $r = hexdec(substr($color, 0, 2));
        $g = hexdec(substr($color, 2, 2));
        $b = hexdec(substr($color, 4, 2));
        return array($r, $g, $b);
    }
}
