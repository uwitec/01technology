<?php
require(dirname(__FILE__) . '/../_common/header.php');
/* @var $ex FLEA_Exception_CacheDisabled */
$cacheDir = realpath($ex->cacheDir);
?>

<h1>FleaPHP �ڲ������޷�ʹ��</h1>

<div class="error">
<h2>��ϸ����ԭ��</h2>
�������������ҳ������Ϊ FleaPHP �ڲ����湦���޷�ʹ�á�<br />
ͨ����������Ϊ������û��ָ��Ӧ�ó������� internalCacheDir ָ��һ�����滺�����ݵ�Ŀ¼��<br />
����ָ����Ŀ¼�޷�д���ļ���
</div>

<p>
<strong>ָ���Ļ���Ŀ¼��</strong>
<?php if ($ex->cacheDir == ''): ?>
û��ָ������Ŀ¼
<?php elseif ($cacheDir == ''): ?>
�����ڵĻ���Ŀ¼��<?php echo h($ex->cacheDir); ?>��
<?php else: ?>
<?php echo h($cacheDir); ?>
<?php endif; ?>
</p>

<div class="tip">
<h2>�����</h2>
�������Ƿ��Ѿ�ָ���˻���Ŀ¼�����߻���Ŀ¼�Ƿ���ڣ����ҿ��Դ����ļ���
</div>

<p></p>

<div class="track">
<?php __error_dump_trace($ex); ?>
</div>
