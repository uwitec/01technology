<?php echo '<?php'; ?>


// {{{ includes
FLEA::loadClass('FLEA_Db_TableDataGateway');
// }}}

class <?php echo $tableClass; ?> extends FLEA_Db_TableDataGateway
{
    var $tableName = '<?php echo $tableName; ?>';
    var $primaryKey = '<?php echo $primaryKey; ?>';

}
