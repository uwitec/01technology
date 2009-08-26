<?php echo '<?php'; ?>


// {{{ includes
FLEA::loadClass('FLEA_Db_ActiveRecord');
// }}}

class <?php echo $modelClass; ?> extends FLEA_Db_ActiveRecord
{

<?php foreach ($mapping as $prop => $field): ?>
    /**
     * <?php echo !empty($field['description']) ? $field['description'] : 'Table field: ' . $field['name']; echo "\n"; ?>
     *
     * @var <?php echo $field['phpType']; echo "\n"; ?>
     */
    var $<?php echo $prop; ?> = null;

<?php endforeach; ?>

    /**
     * 返回对象对应的表数据入口，及对象聚合等信息
     *
     * @return array
     */
    function define()
    {
        return array(
            'tableClass' => '<?php echo $tableClass; ?>',
            'propertiesMapping' => array(
<?php
foreach ($mapping as $prop => $field):
$spc = str_repeat(' ', $len - strlen($prop));
?>
                '<?php echo $prop; ?>'<?php echo $spc; ?> => '<?php echo $field['name']; ?>',
<?php endforeach; ?>
            ),
        );
    }
}
