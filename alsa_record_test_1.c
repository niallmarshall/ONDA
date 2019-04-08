#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

main (int argc, char *argv[])
{
  int i;
  int err;
  char *buffer;
  int buffer_frames = 128;
  unsigned int rate = 44100;
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *hw_params;
	snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;

  FILE *sig_FOut;


  if ((err = snd_pcm_open (&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n",
             argv[1],
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface opened\n");

  if ((sig_FOut = fopen(argv[2], "wb")) == NULL)
  {
  	printf("cannot open audio file for recording\n");
  	exit (1);
  }

  fprintf(stdout, "audio file for recording opened\n");

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params allocated\n");

  if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params initialized\n");

  if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params access setted\n");

  if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, format)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params format setted\n");

  if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params rate setted\n");

  if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params channels setted\n");

  if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "hw_params setted\n");

  snd_pcm_hw_params_free (hw_params);

  fprintf(stdout, "hw_params freed\n");

  if ((err = snd_pcm_prepare (capture_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
             snd_strerror (err));
    exit (1);
  }

  fprintf(stdout, "audio interface prepared\n");

  buffer = malloc(128 * snd_pcm_format_width(format) / 8 * 2);

  fprintf(stdout, "buffer allocated\n");

  while (1)
  {
    if ((err = snd_pcm_readi (capture_handle, buffer, buffer_frames)) != buffer_frames)
	{
      fprintf (stderr, "read from audio interface failed (%s)\n",
               err, snd_strerror (err));
      exit (1);
    }
    //fprintf(stdout, "read %d done\n", i);
	fwrite(buffer, (buffer_frames*2), sizeof(short), sig_FOut);
    fprintf(stdout, "recording...\r");
  }

  free(buffer);

  fprintf(stdout, "buffer freed\n");

  snd_pcm_close (capture_handle);
  fprintf(stdout, "audio interface closed\n");

  exit (0);
}
