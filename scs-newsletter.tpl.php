<div id="newsletter">
  <div id="toc">
    <?php if ($toc): ?>
      <?php echo $toc; ?>
    <?php endif; ?>
  </div>
  <?php foreach ($nodes as $node): ?>
    <?php echo render($node) ?>
  <?php endforeach; ?>
</div>
