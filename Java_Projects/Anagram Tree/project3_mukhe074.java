
class AnagramTree
{
  private TreeNode head;
  private class TreeNode
  {
    private byte[] summary;
    private WordNode words;
    private TreeNode right;
    private TreeNode left;

    private TreeNode( byte[] summary, WordNode words)
    {
      this.summary = summary;
      this.words = words;
      right = null;
      left = null;
    }
  }
  private class WordNode
  {
    private String word;
    private WordNode next;

    private WordNode( String word, WordNode next)
    {
      this.word = word;
      this.next = next;
    }
  }
  public AnagramTree()
  {
    byte[] temp =  new byte[26];
    WordNode temp2 = new WordNode("",null);
    head = new TreeNode(temp, temp2);
  }
  public void add(String word)
  {
    byte[] summary = stringToSummary(word);
    TreeNode subtree = head;
    WordNode temp;
    while (true)
    {
      int comparison = compareSummaries(subtree.summary,summary);
      if (comparison == 0)
      {
        temp = subtree.words;
        while (true)
        {
          if (temp.word.equals(word))
          {
            return;
          }
          if (temp.next == null)
          {
            temp.next = new WordNode(word,null);
            return;
          }
          temp = temp.next;
        }
      }
      else if (comparison < 0)
      {
        if (subtree.right == null)
        {
          subtree.right = new TreeNode(summary, new WordNode (word,null));
          return;
        }
        subtree = subtree.right;
      }
      else if (comparison > 0)
      {
        if (subtree.left == null)
        {
          subtree.left = new TreeNode(summary, new WordNode (word,null));
          return;
        }
        subtree = subtree.left;
      }
    }
  }
  public void anagrams()
  {
    TreeNode subtree = head.right;
    preorder(subtree);

  }
  private void preorder(TreeNode subtree)
  {
    if (subtree != null)
    {
      //if (subtree.words.word == subtree.words.next.word))
      //{}
      if (subtree.words.next != null)
      {
        System.out.print(subtree.words.word);
        subtree.words = subtree.words.next;
        while (subtree.words != null)
        {
          System.out.print(" ");
          System.out.print(subtree.words.word);
          subtree.words = subtree.words.next;
        }
        System.out.println("");
      }
      preorder(subtree.left);
      preorder(subtree.right);
    }
  }
  private int compareSummaries(byte[] left, byte[] right)
  {
    int i = 0;
    for (i=0; i < 26; i++)
    {
      if (left[i] != right[i])
      {
        return (left[i] - right[i]);
      }
    }
    return 0;
  }
  private byte[] stringToSummary (String word)
  {
    byte[] summary = new byte[26];
    for (int i =0; i< word.length(); i ++)
    {
      char c = word.charAt(i);
      summary[(c - 'a')] += 1;
    }
    return summary;
  }
}
class Anagrammer
{
  public static void main(String [] args)
  {
    AnagramTree tree = new AnagramTree();
    Words w = new Words(args[0]);
    while (w.hasNext())
    {
      tree.add(w.next());
    }
    tree.anagrams();
  }
}
