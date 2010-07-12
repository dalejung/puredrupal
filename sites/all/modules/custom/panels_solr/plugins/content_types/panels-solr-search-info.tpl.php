<?php
// $Id$

// Expose as much as I can via preprocess
// This should overriden 
?>

<?php if(isset($result_count)): ?>
  <?php print $start; ?> of <?php print $end; ?>
<?php endif; ?>

<?php if(isset($result_count)): ?>
  <?php print $result_count; ?>
<?php endif; ?>

<?php if($search_terms): ?>
  <?php print $search_terms; ?>
<?php endif; ?>
