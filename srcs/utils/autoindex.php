#!/usr/bin/php

<?PHP
  function getFileList()
  {
    // array to hold return value
    $retval = [];

    // open pointer to directory and read list of files
    $d = @dir("./") or die("getFileList: Failed opening directory for reading");
    while(FALSE !== ($entry = $d->read())) {
      // skip hidden files
      if($entry{0} == ".") continue;
      if(is_dir("{$entry}")) {
        $retval[] = [
          'name' => "{$entry}/",
          'type' => filetype("{$entry}"),
          'size' => 0,
          'lastmod' => filemtime("{$entry}")
        ];
      } elseif(is_readable("{$entry}")) {
        $retval[] = [
          'name' => "{$entry}",
          'type' => mime_content_type("{$entry}"),
          'size' => filesize("{$entry}"),
          'lastmod' => filemtime("{$entry}")
        ];
      }
    }
    $d->close();

    return $retval;
  }

  $dirlist = getFileList();

  echo "<table border=\"1\">\n";
  echo "<thead>\n";
  echo "<tr><th>Name</th><th>Type</th><th>Size</th><th>Last Modified</th></tr>\n";
  echo "</thead>\n";
  echo "<tbody>\n";
  foreach($dirlist as $file) {
    echo "<tr>\n";
    echo "<td>{$file['name']}</td>\n";
    echo "<td>{$file['type']}</td>\n";
    echo "<td>{$file['size']}</td>\n";
    echo "<td>",date('r', $file['lastmod']),"</td>\n";
    echo "</tr>\n";
  }
  echo "</tbody>\n";
  echo "</table>\n\n";

?>