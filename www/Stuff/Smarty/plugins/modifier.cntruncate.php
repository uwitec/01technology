<?php
function smarty_modifier_cntruncate($string, $strlen = 20, $etc = '...', $keep_first_style = false)
{
         $strlen = $strlen*3;
         $string = trim($string);
         if ( strlen($string) <= $strlen )         {
                 return $string;
         }
         $str = strip_tags($string);
         $j = 0;
     for($i=0;$i<$strlen;$i++) {
       if(ord(substr($str,$i,1))>0xa0){ $i+=2; $j+=3;}
       else {$j++;}
     }
     $rstr=substr($str,0,$j);
     if (strlen($str)>$strlen ) {$rstr .= $etc;} 
         if ( $keep_first_style == true && ereg('^<(.*)>$',$string) )         {
                 if ( strlen($str) <= $strlen )         {
                         return $string;
                 }
                 $start_pos = strpos($string,substr($str,0,4));
                 $end_pos = strpos($string,substr($str,-4));
                 $end_pos = $end_pos+4;
                 $rstr = substr($string,0,$start_pos) . $rstr . substr($string,$end_pos,strlen($string));
         }
     return $rstr; 
}

?>