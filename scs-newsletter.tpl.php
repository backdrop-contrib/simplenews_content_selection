<div id="newsletter">
  <?php if ($toc): ?>
    <div id="toc">
      <?php echo $toc; ?>
    </div>
  <?php endif; ?>
  <?php foreach ($nodes as $node): ?>
    <?php echo render($node) ?>
  <?php endforeach; ?>
</div>
